#pragma once
#include "CoreMinimal.h"
#include "RangedWeapon.h"
#include "ProjectileWeapon.generated.h"

UCLASS()
class MECHRPG_API UProjectileWeapon : public URangedWeapon
{
	GENERATED_BODY()
public:
	FProjectileWeaponData GetProjectileWeaponData() const { return projectileWeaponData; }
	void SetProjectileWeaponData(FProjectileWeaponData data) { projectileWeaponData = data; currentAmmo = data.magazineSize;  }
	virtual void UseWeapon(const FVector& LookAtRotation) override;

	FTimerHandle TimerHandle_ReloadExpired;
	void ReloadEExpired();
protected:
	FProjectileWeaponData projectileWeaponData;
	int32 currentAmmo;
	
};