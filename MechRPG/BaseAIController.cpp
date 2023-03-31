#include "BaseAIController.h"
#include "BaseProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "RPGGameInstance.h"
#include "Events/BaseEvent.h"
#include "Events/HealthChangeEvent.h"
#include "Events/RPGEventManager.h"
#include "BaseCharacter.h"
#include "Damagable.h"
#include "Abilities/BaseAbility.h"
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
			FindViableCombatLocationRequest.Execute(EEnvQueryRunMode::SingleResult, this, &ABaseAIController::MyQueryFinished);

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

void ABaseAIController::MyQueryFinished(TSharedPtr<FEnvQueryResult> Result)
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

	if (target != NULL)
	{
		const FVector targetLocation = target->asActor()->GetActorLocation();
		const UWeapon* weapon = mCurrentWeapon();
		bool abilityUsed = false;

		LookAt(targetLocation);

		// Can we see our current target?
		if (canSee) {

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
					FindViableCombatLocationRequest.Execute(EEnvQueryRunMode::SingleResult, this, &ABaseAIController::MyQueryFinished);
				}
			}
		}
		// We can't see the target, make sure we're not already trying to move to the target
		else if (!GetCharacter()->GetCharacterMovement()->IsMovementInProgress())
		{
			// Move to the last known location
			lastKnowLocation = target->asActor()->GetActorLocation();
			FindViableCombatLocationRequest.Execute(EEnvQueryRunMode::SingleResult, this, &ABaseAIController::MyQueryFinished);
		}
	}
}

void ABaseAIController::AttackLocation(FVector FireDirection)
{
	mCurrentWeapon()->UseWeapon(FireDirection);
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
	TArray<EEventType> types;
	types.Add(EEventType::HealthChange);
	types.Add(EEventType::CombatState);
	gameIn->GetEventManager()->RegisterListener(types, this);

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

void ABaseAIController::EventTriggered(UBaseEvent* inEvent)
{
	// Only change target if our current target is null or dead, and the event is of type health change
	if ((target == NULL || target->IsDead()) && inEvent->GetEventType() == EEventType::HealthChange) {
		UHealthChangeEvent* hce = Cast<UHealthChangeEvent>(inEvent);

		// Only trigger after health changed, the change isn't a heal and the owner of change is our Pawn
		if (!hce->GetPreChange() && !hce->GetChange().heals && hce->GetEventOwner() == GetCharacter()) {
			target = hce->GetChange().source;

			//UE_LOG(HealthChangeLog, Log, TEXT("Owner Took %f damage"), hce->GetChange().changeAmount);
		}
	}
}