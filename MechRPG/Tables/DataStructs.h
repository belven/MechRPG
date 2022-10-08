#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataStructs.generated.h"

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

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY()
public:
	int32 ID;
	FString name;
	EItemType type;
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
	static bool GetBoolean(FString value);
};
