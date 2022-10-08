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

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY()
public:
	int32 ID;
	FString name;
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
};

UCLASS()
class MECHRPG_API UDataStructs : public UObject
{
	GENERATED_BODY()
public:
	static EWeaponType GetWeaponType(FString typeName);
	static bool GetBoolean(FString value);
};
