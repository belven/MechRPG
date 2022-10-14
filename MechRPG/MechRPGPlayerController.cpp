#include "MechRPGPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "MechRPGCharacter.h"
#include "Items/Weapon.h"
#include "Items/WeaponCreator.h"
#include "Kismet/KismetMathLibrary.h"

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
}

void AMechRPGPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	mAsMech(aPawn)->SetEquippedWeapon(UWeaponCreator::CreateWeapon(4, GetWorld()));
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