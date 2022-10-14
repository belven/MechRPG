#pragma once
#include "CoreMinimal.h"
#include "Weapon.h"
#include "MechRPG/Tables/DataStructs.h"
#include "RangedWeapon.generated.h"

UCLASS()
class MECHRPG_API URangedWeapon : public UWeapon
{
	GENERATED_BODY()
public:
	FRangedWeaponData GetRangedWeaponData() const	{		return rangedWeaponData;	}
	void SetRangedWeaponData(FRangedWeaponData data) { rangedWeaponData = data; }
protected:
	FRangedWeaponData rangedWeaponData;
};