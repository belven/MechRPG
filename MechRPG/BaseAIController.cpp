#include "BaseAIController.h"
#include "BaseProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "RPGGameInstance.h"
#include "Events/BaseEvent.h"
#include "Events/HealthChangeEvent.h"
#include "Events/RPGEventManager.h"
#include "BaseCharacter.h"
#include "Damagable.h"
#include "PatrolPath.h"
#include "Abilities/BaseAbility.h"
#include "AI/NavigationSystemBase.h"
#include "Components/SplineComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#define mActorLocation GetCharacter()->GetActorLocation()
#define mActorRotation GetCharacter()->GetActorRotation()
#define mCurrentWeapon() GetBaseCharacter()->GetEquippedWeapon()
#define mSphereTraceMulti(start, end, radius, ignore, hits) UKismetSystemLibrary::SphereTraceMulti(GetWorld(), start, end, radius, ETraceTypeQuery::TraceTypeQuery1, false, ignore, EDrawDebugTrace::ForOneFrame, hits,true)

ABaseAIController::ABaseAIController() : Super()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	PerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());
	PerceptionComponent->ConfigureSense(*sightConfig);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::TargetPerceptionUpdated);

	FindWeaponLocationQuery = CreateDefaultSubobject<UEnvQuery>(TEXT("AIPerception Query"));
	static ConstructorHelpers::FObjectFinder<UEnvQuery> playerLocationQuery(TEXT("EnvQuery'/Game/TopDown/EQS_FindPlayer.EQS_FindPlayer'"));

	if (playerLocationQuery.Succeeded())
	{
		FindWeaponLocationQuery = playerLocationQuery.Object;
	}

	currentPath = NULL;
	currentPathPoint = 0;
}

void ABaseAIController::LookAt(FVector lookAtLocation)
{
	FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(mActorLocation, lookAtLocation);
	lookAt.Pitch = mActorRotation.Pitch;
	lookAt.Roll = mActorRotation.Roll;
	GetCharacter()->SetActorRotation(lookAt);
}

void ABaseAIController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	AICharacter = mAsBaseCharacter(aPawn);

	URPGGameInstance* gameIn = GameInstance(GetWorld());
	//TArray<EEventType> types;
	//types.Add(EEventType::PostHealthChange);
	//types.Add(EEventType::CombatState);
	//gameIn->GetEventManager()->RegisterListener(types, this);
	gameIn->GetEventManager()->OnEventTriggered.AddUniqueDynamic(this, &ABaseAIController::EventTriggered);
	constexpr int32 range = 3000;

	// Set up sight config for AI perception
	sightConfig->SightRadius = range * 0.9;
	sightConfig->LoseSightRadius = range;
	sightConfig->PeripheralVisionAngleDegrees = 90.0f;

	// This section is important, as without setting at least bDetectNeutrals to true, the AI will never perceive anything
	// Still not tried to set this up correctly at all
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionComponent->ConfigureSense(*sightConfig);

	// Add the AIs character to things that can be perceived by this sight config.
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, sightConfig->GetSenseImplementation(), aPawn);

	// Set up our EQS query 
	FindViableCombatLocationRequest = FEnvQueryRequest(FindWeaponLocationQuery, this);
}

void ABaseAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// Is the actor our current target?
	if (*target == Actor)
	{
		// Update the lastKnowLocation, as regardless of if we see the target or not, we might re-adjust our movement to a better location
		lastKnowLocation = Stimulus.StimulusLocation;

		// Did we loose sight of them?
		if (!Stimulus.WasSuccessfullySensed())
		{
			// We can't see them
			canSee = false;

			// Move to their last seen location
			MoveToCombatLocation();

			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I don't see you!");
		}
		// Have we seen them again?
		else {
			// We can see them again
			canSee = true;

			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "I see you!");
		}

	}
	// If we don't have a target, then check if this is a new viable target
	else if (target == NULL)
	{
		// Get the Actors team, if it has one, and check if we're enemies
		ITeam* otherTeam = Cast<ITeam>(Actor);

		if (otherTeam != NULL) {

			// Are we enemies with the perceived actor?
			if (AICharacter->GetRelationship(otherTeam, AICharacter->GetGameInstance()) == ERelationshipType::Enemy)
			{
				// Update our target and set that we can see them, we can assume that, if the actor is a team, it's also damagable
				target = Cast<IDamagable>(Actor);
				canSee = true;

				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "I see you!");
			}
		}
	}
}

void ABaseAIController::WeaponLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	// Did we find a new location to move to?
	if (Result->IsSuccessful()) {

		// Get the first item as a location, this will be the highest scoring location in the array, as it orders items by score
		const FVector loc = Result->GetItemAsLocation(0);

		// Move to the location found
		MoveToLocation(loc);
	}
}

void ABaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetBaseCharacter() != NULL && GetBaseCharacter()->IsDead())
	{
		KillAI();
	}
	else if (target != NULL)
	{
		CalculateCombat();
	}
	else
	{
		Patrol();
	}
}

void ABaseAIController::Patrol()
{
	// Check if we aren't moving or we have an invalid Path
	// (the currentPathPoint is set to -1 when we didn't find any patrol paths in the world)
	if(GetPathFollowingComponent()->GetStatus() != EPathFollowingStatus::Moving && currentPathPoint > -1)
	{
		// Do we already have a patrol path
		if(currentPath == NULL)
		{
			// Get the first patrol path from the game instance
			currentPath = GameInstance(GetWorld())->paths[0];

			// Check if the path is valid
			if(currentPath != NULL && currentPath->GetSpline()->GetNumberOfSplinePoints() < 1)
			{
				currentPath = NULL;

				// Set path point to -1, as there aren't any valid patrol paths and we don't keep checking every tick
				currentPathPoint = -1;
			}
			else
			{
				currentPathPoint = 0;				
			}
		}

		if (currentPath != NULL)
		{
			// Get the spline of the patrol path
			const USplineComponent* spline = currentPath->GetSpline();

			// Ensure the path point doesn't exceed the amount of points
			if(currentPathPoint > spline->GetNumberOfSplinePoints() - 1)
			{
				currentPathPoint = 0;
			}

			// Get the next spline point to move to
			const FVector loc = spline->GetLocationAtSplinePoint(currentPathPoint, ESplineCoordinateSpace::World);

			// Move to the spline point
			MoveToLocation(loc);

			// Increment the path point, to move onto the next one
			currentPathPoint++;
		}
	}
}

void ABaseAIController::KillAI()
{
	StopMovement();
	SetActorTickEnabled(false);
	target = NULL;
}

void ABaseAIController::CalculateCombat()
{
	const FVector targetLocation = target->asActor()->GetActorLocation();
	const UWeapon* weapon = mCurrentWeapon();
	bool abilityUsed = false;

	// Can we see our current target?
	if (canSee) {
		LookAt(targetLocation);

		// Check for any abilities we can use
		for (UBaseAbility* ability : AICharacter->GetAbilities())
		{
			if (!ability->IsOnCooldown())
			{
				ability->Use(target);
				abilityUsed = true;
				break;
			}
		}

		// if we haven't used an ability and have a valid weapon, attack the target
		if (!abilityUsed && weapon != NULL) {

			// Check we're in range of the target
			if (FVector::Dist(mActorLocation, targetLocation) <= weapon->GetWeaponData().range) {
				AttackLocation(mActorRotation.Vector());
			}
			// Otherwise move towards the targets current location
			else
			{
				// We updated the lastKnowLocation here, as we can still see the target and simply need to move forwards to attack again
				lastKnowLocation = target->asActor()->GetActorLocation();
				MoveToCombatLocation();
			}
		}
	}
	// We can't see the target, make sure we're not already trying to move to the target
	else if (!GetCharacter()->GetCharacterMovement()->IsMovementInProgress())
	{
		// Move to the last known location
		LookAt(lastKnowLocation);
		lastKnowLocation = target->asActor()->GetActorLocation();
		MoveToCombatLocation();
	}
}

void ABaseAIController::MoveToCombatLocation()
{
	FindViableCombatLocationRequest.Execute(EEnvQueryRunMode::SingleResult, this, &ABaseAIController::WeaponLocationQueryFinished);
}

void ABaseAIController::AttackLocation(FVector FireDirection)
{
	mCurrentWeapon()->UseWeapon(FireDirection);
}

void ABaseAIController::FindNewTarget()
{
	TArray<AActor*> actorsSensed;

	// Get all the actors we've sensed in our lifetime
	PerceptionComponent.Get()->GetKnownPerceivedActors(sightConfig->GetSenseImplementation(), actorsSensed);

	for (AActor* actor : actorsSensed)
	{
		if (actor->Implements<UTeam>())
		{
			ITeam* team = Cast<ITeam>(actor);
			IDamagable* damagable = Cast<IDamagable>(actor);

			// Check if the actor is both damagable and an enemy
			if (damagable->IsAlive()
				&& AICharacter->GetRelationship(team, AICharacter->GetGameInstance()) == ERelationshipType::Enemy)
			{
				// Move to the targets last known location
				// As the target is set in here, the code in tick, in the canSee = false section, will update the lastKnowLocation to their current location
				// This means the AI will move to the targets exact location, rather than just their last location
				target = damagable;
				lastKnowLocation = PerceptionComponent.Get()->GetActorInfo(*actor)->GetLastStimulusLocation();
				MoveToLocation(lastKnowLocation);
			}
		}
	}
}

void ABaseAIController::EventTriggered(UBaseEvent* inEvent)
{
	// Check if the event is a post health change
	if (inEvent->GetEventType() == EEventType::PostHealthChange) {
		UHealthChangeEvent* hce = Cast<UHealthChangeEvent>(inEvent);

		// If our target is NULL, we can check if we're being attacked and maybe assign a new target
		if (target == NULL) {

			// Only trigger after health changed, the change isn't a heal and the owner of change is our Pawn and if the source is alive still
			// It's possible we take damage from a dead source
			if (!hce->GetChange().heals && hce->GetEventOwner() == GetCharacter() && !hce->GetChange().source->IsDead()) {
				target = hce->GetChange().source;
			}
		}
		// Check if the change is damage
		// Check if the change is against our target
		// Check if our target is dead
		else if (!hce->GetChange().heals && hce->GetEventOwner() == *target && target->IsDead())
		{
			// Clear our Target
			target = NULL;

			// clear sight
			canSee = false;

			// Try to find a new target
			FindNewTarget();
		}
	}
}