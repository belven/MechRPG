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
	void SetProjectileWeaponData(FProjectileWeaponData data) { projectileWeaponData = data; }
	virtual void UseWeapon(const FVector& location) override;
protected:
	FProjectileWeaponData projectileWeaponData;
	
};