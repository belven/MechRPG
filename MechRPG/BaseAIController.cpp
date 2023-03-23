#include "BaseAIController.h"

#include "BaseProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "RPGGameInstance.h"
#include "Events/BaseEvent.h"
#include "Events/HealthChangeEvent.h"
#include "Events/RPGEventManager.h"
#include "MechRPGCharacter.h"
#include "Damagable.h"
#include "Abilities/BaseAbility.h"
#include "Components/CapsuleComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#define mActorLocation GetCharacter()->GetActorLocation()
#define mActorRotation GetCharacter()->GetActorRotation()
#define mTargetActor() Cast<AActor>(target)
#define mCurrentWeapon() mAsMech(GetCharacter())->GetEquippedWeapon()
#define mSphereTraceMulti(start, end, radius, ignore, hits) UKismetSystemLibrary::SphereTraceMulti(GetWorld(), start, end, radius, ETraceTypeQuery::TraceTypeQuery1, false, ignore, EDrawDebugTrace::ForOneFrame, hits,true)

ABaseAIController::ABaseAIController() : Super()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	PerceptionComponent->ConfigureSense(*sightConfig);
	PerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ABaseAIController::PerceptionUpdated);

	MyQuery = CreateDefaultSubobject<UEnvQuery>(TEXT("AIPerception Query"));
	static ConstructorHelpers::FObjectFinder<UEnvQuery> playerLocationQuery(TEXT("EnvQuery'/Game/TopDown/EQS_FindPlayer.EQS_FindPlayer'"));

	if (playerLocationQuery.Succeeded())
	{
		MyQuery = playerLocationQuery.Object;
	}

}

void ABaseAIController::PerceptionUpdated(const TArray<AActor*>& testActors)
{
	if (target == NULL) {
		for (AActor* other : testActors)
		{
			AMechRPGCharacter* mech = mAsMech(other);
			if (mech != NULL) {
				if (mech->GetRelationship(mAsMech(GetCharacter()), mech->GetGameInstance()) == ERelationshipType::Enemy)
				{
					target = mech;
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I see you!");
					break;
				}
			}
		}
	}
	else
	{
		FActorPerceptionBlueprintInfo info;
		PerceptionComponent->GetActorsPerception(GetCharacter(), info);

		for (FAIStimulus aiStim : info.LastSensedStimuli)
		{
			if (aiStim.Type == sightConfig->GetSenseID() && !aiStim.WasSuccessfullySensed())
			{
				const FVector targetLocation = mTargetActor()->GetActorLocation();
				MyQueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &ABaseAIController::MyQueryFinished);
			}
		}
	}
}

void ABaseAIController::MyQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (Result->IsSuccessful()) {
		FVector loc = Result->GetItemAsLocation(0);
		MoveToLocation(loc);
	}
}

void ABaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target != NULL)
	{
		const FVector targetLocation = mTargetActor()->GetActorLocation();
		const UWeapon* weapon = mCurrentWeapon();
		bool abilityUsed = false;

		LookAt(targetLocation);

		TArray<FHitResult> hits = CheckLoSToTarget();

		bool canSee = true;

		for (FHitResult hit : hits) {
			if (!hit.GetActor()->Implements<UDamagable>()) {
				MyQueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &ABaseAIController::MyQueryFinished);
				canSee = false;
				break;
			}
		}

		if (canSee) {
			for (UBaseAbility* ability : mAsMech(GetCharacter())->GetAbilities())
			{
				if (!ability->IsOnCooldown())
				{
					ability->Use(target);
					abilityUsed = true;
				}
			}

			if (!abilityUsed && weapon != NULL) {

				if (FVector::Dist(mActorLocation, targetLocation) <= weapon->GetWeaponData().range) {
					StopMovement();
					FireShot(mActorRotation.Vector());
				}
				else
				{
					MyQueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &ABaseAIController::MyQueryFinished);

				}
			}
		}
	}
}

TArray<FHitResult> ABaseAIController::CheckLoSToTarget() {
	TArray<AActor*> ignore;
	ignore.Add(GetCharacter());

	const ACharacter* targetA = Cast<ACharacter>(target);

	TArray<FHitResult> hits;
	const FVector start = mActorLocation;
	const FVector end = targetA->GetActorLocation();
	const float radius = targetA->GetCapsuleComponent()->GetScaledCapsuleRadius() * 0.7;

	//mSphereTraceMulti(start, end, radius, ignore, hits, true);

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), start, end, radius, ETraceTypeQuery::TraceTypeQuery1, false, ignore, EDrawDebugTrace::ForOneFrame, hits, true);

	return hits;
}

bool ABaseAIController::CanSee(AActor* other, FVector startLoc) {
	//FHitResult hit = CheckLoSToTarget(startLoc, other->GetActorLocation());

	//// Do we have line of sight to our target?
	//if (hit.GetActor() == other)
	//{
	//	return true;
	//}
	return false;
}

void ABaseAIController::FireShot(FVector FireDirection)
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

	URPGGameInstance* gameIn = GameInstance(GetWorld());
	TArray<EEventType> types;
	types.Add(EEventType::HealthChange);
	types.Add(EEventType::CombatState);
	gameIn->GetEventManager()->RegisterListener(types, this);

	constexpr float sightRange = 6000.0f;

	sightConfig->SightRadius = sightRange;
	sightConfig->LoseSightRadius = sightRange;
	sightConfig->PeripheralVisionAngleDegrees = 90.0f;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionComponent->ConfigureSense(*sightConfig);

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, sightConfig->GetSenseImplementation(), aPawn);

	MyQueryRequest = FEnvQueryRequest(MyQuery, this);
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