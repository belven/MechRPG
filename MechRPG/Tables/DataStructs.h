#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataStructs.generated.h"

class AMechRPGCharacter;
UENUM(BlueprintType)
enum class  EWeaponType : uint8 {
	Laser,
	Melee,
	Projectile,
	End
};

UENUM(BlueprintType)
enum class  EItemType : uint8 {
	Weapon,
	Consumable,
	Armour,
	End
};

UENUM(BlueprintType)
enum class  EDamageType : uint8 {
	Energy,
	Kinetic,
	Fire,
	End
};

UENUM(BlueprintType)
enum class  ECharacterType : uint8 {
	Player,
	Neutral,
	Ally,
	Enemy,
	End
};

UENUM(BlueprintType)
enum class  EArmourSlot : uint8 {
	Head,
	Chest,
	Left_Leg,
	RIght_Leg,
	Left_Arm,
	Right_Arm,
	End
};

USTRUCT(BlueprintType)
struct FCombatStateChange
{
	GENERATED_USTRUCT_BODY()
public:
	bool oldState;
	bool newState;

	UPROPERTY()
		UObject* source;
};

USTRUCT(BlueprintType)
struct FHealthChange
{
	GENERATED_USTRUCT_BODY()
public:
	float changeAmount;
	bool heals;
	EDamageType damageType;

	UPROPERTY()
		AMechRPGCharacter* source;
};

USTRUCT(BlueprintType)
struct FLoadoutData
{
	GENERATED_USTRUCT_BODY()
public:
	int32 ID;
	int32 entityID;
	ECharacterType type;
	int32 weaponID;
	int32 headArmourID;
	int32 chestArmourID;
	int32 leftLegArmourID;
	int32 rightLegArmourID;
	int32 leftArmArmourID;
	int32 rightArmArmourID;
	int32 abilityOneID;
	int32 abilityTwoID;
	int32 abilityThreeID;
	int32 health;
	int32 moveSpeed;
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY()
public:
	int32 ID;
	FString name;
	EItemType type;
	int32 maxStack;
};

USTRUCT(BlueprintType)
struct FArmourData
{
	GENERATED_USTRUCT_BODY()
public:
	int32 ID;
	int32 itemID;
	EArmourSlot slot;
};

USTRUCT(BlueprintType)
struct FArmourResistanceData
{
	GENERATED_USTRUCT_BODY()
public:
	int32 ID;
	int32 armourID;
	float resistanceAmount;
	EDamageType type;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()
public:
	int32 ID;
	int32 itemID;
	EWeaponType type;
	float useRate;
	float healthChange;
	bool heals;
	int32 range;
};

USTRUCT(BlueprintType)
struct FMeleeWeaponData
{
	GENERATED_USTRUCT_BODY()
public:
	int32 ID;
	int32 weaponID;
	float cleaveRadius;
};

USTRUCT(BlueprintType)
struct FProjectileWeaponData
{
	GENERATED_USTRUCT_BODY()
public:
	int32 ID;
	int32 rangedWeaponID;
	FString projectileClass;
	int32 magazineSize;
	float reloadSpeed;
};

USTRUCT(BlueprintType)
struct FRangedWeaponData
{
	GENERATED_USTRUCT_BODY()
public:
	int32 ID;
	int32 weaponID;
	float accuracy;
};

USTRUCT(BlueprintType)
struct FLaserWeaponData
{
	GENERATED_USTRUCT_BODY()
public:
	int32 ID;
	int32 rangedWeaponID;
	FString particleEffect;
};

USTRUCT(BlueprintType)
struct FCombinedWeaponData
{
	GENERATED_USTRUCT_BODY()
public:
	FItemData itemData;
	FWeaponData weaponData;
	FLaserWeaponData laserWeaponData;
	FRangedWeaponData rangedWeaponData;
	FMeleeWeaponData meleeWeaponData;
	FProjectileWeaponData projectileWeaponData;
};

UCLASS()
class MECHRPG_API UDataStructs : public UObject
{
	GENERATED_BODY()
public:
	static EWeaponType GetWeaponType(FString typeName);
	static EItemType GetItemType(FString typeName);
	static EArmourSlot GetArmourSlot(FString typeName);
	static EDamageType GetDamageType(FString typeName);
	static ECharacterType GetCharacterType(FString typeName);
	static bool GetBoolean(FString value);
};
