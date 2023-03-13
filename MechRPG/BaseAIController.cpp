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
#include "Kismet/KismetMathLibrary.h"

#define mActorLocation GetCharacter()->GetActorLocation()
#define mActorRotation GetCharacter()->GetActorRotation()
#define mTargetActor() Cast<AActor>(target)
#define mCurrentWeapon() mAsMech(GetCharacter())->GetEquippedWeapon()

void ABaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target != NULL)
	{
		const FVector targetLocation = mTargetActor()->GetActorLocation();
		const UWeapon* weapon = mCurrentWeapon();
		bool abilityUsed = false;

		LookAt(targetLocation);

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
				MoveToLocation(targetLocation);
			}
		}
	}
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