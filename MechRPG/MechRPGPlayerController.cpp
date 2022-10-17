#include "MechRPGPlayerController.h"
#include "MechRPG.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "MechRPGCharacter.h"
#include "RPGGameInstance.h"
#include "Events/CombatStateEvent.h"
#include "Events/HealthChangeEvent.h"
#include "Events/RPGEventManager.h"
#include "Items/Weapon.h"
#include "Items/WeaponCreator.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MechRPG/RPGGameInstance.h"

#define mActorLocation GetCharacter()->GetActorLocation()
#define mActorRotation GetCharacter()->GetActorRotation()
#define mAsMech(character) Cast<AMechRPGCharacter>(character)

const FName AMechRPGPlayerController::MoveForwardBinding("MoveForward");
const FName AMechRPGPlayerController::MoveRightBinding("MoveRight");
const FName AMechRPGPlayerController::FireForwardBinding("FireForward");
const FName AMechRPGPlayerController::FireRightBinding("FireRight");

AMechRPGPlayerController::AMechRPGPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	MoveSpeed = 800.0f;
}

void AMechRPGPlayerController::EventTriggered(UBaseEvent* inEvent)
{
	if (inEvent->GetEventType() == EEventType::HealthChange) {
		UHealthChangeEvent* hce = Cast<UHealthChangeEvent>(inEvent);
		UE_LOG(HealthChangeLog, Log, TEXT("Owner Took %d damage"), hce->GetChange().changeAmount);
	} else if (inEvent->GetEventType() == EEventType::CombatState) {
		UCombatStateEvent* cse = Cast<UCombatStateEvent>(inEvent);
		UE_LOG(HealthChangeLog, Log, TEXT("Combat State changed to %s"), cse->GetStateChange().newState ? TEXT("true") : TEXT("false"));
	}
}

void AMechRPGPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	mAsMech(aPawn)->SetEquippedWeapon(UWeaponCreator::CreateWeapon(4, GetWorld()));

	URPGGameInstance* gameIn = GameInstance(GetWorld());
	TArray<EEventType> types;
	types.Add(EEventType::HealthChange);
	types.Add(EEventType::CombatState);
	gameIn->GetEventManager()->RegisterListener(types, this);
}

void AMechRPGPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMechRPGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	GetHitResultUnderCursor(ECC_WorldStatic, false, Hit);

	if (bMovementActive)
	{
		FollowTime += DeltaTime;

		APawn* const MyPawn = GetPawn();
		if (MyPawn)
		{
			const FVector WorldDirection = (Hit.Location - MyPawn->GetActorLocation()).GetSafeNormal();
			MyPawn->AddMovementInput(WorldDirection, 1.f, false);
		}
	} else
	{
		CalculateMovement(DeltaTime);
	}

	if (isFiring) {
		FiringTime += DeltaTime;

		if (FiringTime > ShortPressThreshold) {
			if (mechTarget != NULL)
			{
				mechTarget = NULL;
			}

			LookAt(Hit.ImpactPoint);
			FireShot(mActorRotation.Vector());
		}
	}
	else if (mechTarget != NULL)
	{
		LookAt(mechTarget->GetActorLocation());
		FireShot(mActorRotation.Vector());
	}
	else
	{
		LookAt(Hit.ImpactPoint);
	}
}

void AMechRPGPlayerController::LookAt(FVector lookAtLocation)
{
	FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(mActorLocation, lookAtLocation);
	lookAt.Pitch = mActorRotation.Pitch;
	lookAt.Roll = mActorRotation.Roll;

	GetCharacter()->SetActorRotation(lookAt);
}

void AMechRPGPlayerController::FireShot(FVector FireDirection)
{
	mAsMech(GetCharacter())->GetEquippedWeapon()->UseWeapon(FireDirection);
}

void AMechRPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Fire", IE_Pressed, this, &AMechRPGPlayerController::Fire);
	InputComponent->BindAction("Fire", IE_Released, this, &AMechRPGPlayerController::StopFiring);

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AMechRPGPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AMechRPGPlayerController::OnSetDestinationReleased);

	InputComponent->BindAxis(MoveForwardBinding);
	InputComponent->BindAxis(MoveRightBinding);
}

void AMechRPGPlayerController::Fire()
{
	FiringTime = 0.f;
	isFiring = true;
}

void AMechRPGPlayerController::StopFiring()
{
	isFiring = false;

	if (FiringTime <= ShortPressThreshold)
	{
		FHitResult HitPawn;
		GetHitResultUnderCursor(ECC_Pawn, false, HitPawn);
		AMechRPGCharacter* mech = Cast<AMechRPGCharacter>(HitPawn.GetActor());

		if (mech != NULL) {
			mechTarget = mech;
		}
	}
}

void AMechRPGPlayerController::CalculateMovement(float DeltaSeconds) const
{
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding); // W S
	const float RightValue = GetInputAxisValue(MoveRightBinding); // A D

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		GetCharacter()->AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), ForwardValue);
		GetCharacter()->AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), RightValue);
	}
}

void AMechRPGPlayerController::OnSetDestinationPressed()
{
	FollowTime = 0.f;
	bMovementActive = true;
	StopMovement();
}

void AMechRPGPlayerController::OnSetDestinationReleased()
{
	// Player is no longer pressing the input
	bMovementActive = false;

	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We look for the location in the world where the player has pressed the input
		FHitResult HitWorld;
		GetHitResultUnderCursor(ECC_Visibility, true, HitWorld);

		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitWorld.Location);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, HitWorld.Location, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}