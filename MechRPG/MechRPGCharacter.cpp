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
#include "Items/Armour.h"
#include "Items/ArmourCreator.h"
#include "Items/WeaponCreator.h"
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

	maxStats.health = 1000;
	maxStats.moveSpeed = 800;
	currentStats = maxStats;
}

void AMechRPGCharacter::ChangeHealth(FHealthChange& health_change)
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
		currentStats.health += health_change.changeAmount;
	}
	else {
		health_change.changeAmount = GetDamageAfterResistance(health_change.changeAmount, health_change.damageType);
		currentStats.health -= health_change.changeAmount;
	}

	FMath::Clamp(health_change.heals, 0, maxStats.health);

	mEventTriggered(GetGameInstance(), mCreateHealthChangeEvent(this, health_change, false));
}

float  AMechRPGCharacter::GetDamageAfterResistance(float damage, EDamageType type) {
	const float resistance = 100.0f + GetDamageResistance(type);
	const float damageReduction = 100.0f / resistance;
	return damage * damageReduction;
}

int32  AMechRPGCharacter::GetDamageResistance(EDamageType type) {
	int32 total = 0;

	TArray<UArmour*> armour;
	equippedArmour.GenerateValueArray(armour);
	
	for(const UArmour* a : armour)
	{
		for (const FArmourResistanceData ard : a->GetResistances()) {
			if (ard.type == type)
				total += ard.resistanceAmount;
		}
	}

	return total;
}

URPGGameInstance* AMechRPGCharacter::GetGameInstance()
{
	if (gameInstance == NULL)
		gameInstance = GameInstance(GetWorld());
	return gameInstance;
}

void AMechRPGCharacter::EquipArmour(UArmour* armour)
{
	equippedArmour.FindOrAdd(armour->GetData().slot, armour);
}

void AMechRPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	const FLoadoutData ld = GetGameInstance()->GetLoadoutData(1);
	
	SetEquippedWeapon(UWeaponCreator::CreateWeapon(ld.weaponID, GetWorld()));

	EquipArmour(UArmourCreator::CreateArmour(ld.headArmourID, GetWorld()));
	EquipArmour(UArmourCreator::CreateArmour(ld.chestArmourID, GetWorld()));
	EquipArmour(UArmourCreator::CreateArmour(ld.leftArmArmourID, GetWorld()));
	EquipArmour(UArmourCreator::CreateArmour(ld.rightArmArmourID, GetWorld()));
	EquipArmour(UArmourCreator::CreateArmour(ld.leftLegArmourID, GetWorld()));
	EquipArmour(UArmourCreator::CreateArmour(ld.rightLegArmourID, GetWorld()));
}
