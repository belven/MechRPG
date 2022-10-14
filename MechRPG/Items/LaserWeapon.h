#pragma once
#include "CoreMinimal.h"
#include "RangedWeapon.h"
#include "MechRPG/Tables/DataStructs.h"
#include "LaserWeapon.generated.h"

UCLASS()
class MECHRPG_API ULaserWeapon : public URangedWeapon
{
	GENERATED_BODY()
public:
	FLaserWeaponData GetLaserData() const { return laserData; }
	void SetLaserData(FLaserWeaponData data) { laserData = data; }
protected:
	FLaserWeaponData laserData;
};
