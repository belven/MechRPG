#include "MechRPGCharacter.h"
#include "RPGGameInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Events/CombatStateEvent.h"
#include "Events/HealthChangeEvent.h"
#include "Events/RPGEventManager.h"
#include "Kismet/GameplayStatics.h"

AMechRPGCharacter::AMechRPGCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 4000.0f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMechRPGCharacter::ChangeHealth(const FHealthChange& health_change)
{
	mEventTriggered(GetGameInstance(), mCreateHealthChangeEvent(this, health_change, true));

	if (!inCombat && !health_change.heals)
	{
		FCombatStateChange csc;
		csc.source = this;
		csc.oldState = false;
		csc.newState = true;
		inCombat = true;
		mEventTriggered(GetGameInstance(), mCreateCombatStateEvent(this, csc));
	}

	if (health_change.heals) {
		// Restore Health
	}
	else {
		float damageTaken = GetDamageAfterResistance(health_change.changeAmount, health_change.damageType);
		// Take Damage
	}

	mEventTriggered(GetGameInstance(), mCreateHealthChangeEvent(this, health_change, false));
}

float  AMechRPGCharacter::GetDamageAfterResistance(float damage, EDamageType type) {
	float damageReduction = 100 / (100 + GetDamageResistance(type));
	float invertPercent = 1 - damageReduction;
	return damage * invertPercent;
}

int32  AMechRPGCharacter::GetDamageResistance(EDamageType type) {

}

URPGGameInstance* AMechRPGCharacter::GetGameInstance()
{
	if (gameInstance == NULL)
		gameInstance = GameInstance(GetWorld());
	return gameInstance;
}
