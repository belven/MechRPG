#pragma once
#include "CoreMinimal.h"
#include "Item.h"
#include "MechRPG/Tables/DataStructs.h"
#include "Weapon.generated.h"

#define mSpawnProjectile(projectileClass) owner->GetWorld()->SpawnActor<ABaseProjectile>(projectileClass, gunLocation, FireRotation)
#define mSetTimer(handle, method, delay) GetWorld()->GetTimerManager().SetTimer(handle, this, method, delay)
#define mSetTimerWorld(world, handle, method, delay) world->GetTimerManager().SetTimer(handle, this, method, delay)

class AMechRPGCharacter;
class ABaseProjectile;

UCLASS()
class MECHRPG_API UWeapon : public UItem
{
	GENERATED_BODY()
public:
	UWeapon();

	FWeaponData GetWeaponData() const { return weaponData; }
	void SetWeaponData(FWeaponData data) { weaponData = data; }
	virtual void UseWeapon(const FVector& LookAtRotation);
	
	AMechRPGCharacter* GetOwner() const { return owner; }
	void SetOwner(AMechRPGCharacter* val) { owner = val; }

	void ShotTimerExpired();
protected:
	FWeaponData weaponData;
	FVector GunOffset;
	bool canFire;
	FTimerHandle TimerHandle_ShotTimerExpired;

	UPROPERTY()
	AMechRPGCharacter* owner;

	ABaseProjectile* SpawnProjectile(FVector gunLocation, FRotator FireRotation, UClass* projectileClass);
};