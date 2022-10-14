#include "Weapon.h"
#include "../BaseProjectile.h"

UWeapon::UWeapon()
{
	GunOffset = FVector(100.f, 0.f, 0.f);
	canFire = true;
}

void UWeapon::ShotTimerExpired()
{
	canFire = true;
}

ABaseProjectile* UWeapon::SpawnProjectile(FVector gunLocation, FRotator FireRotation, UClass* projectileClass) {
	ABaseProjectile* projectile = mSpawnProjectile(projectileClass);
	projectile->healthChange.changeAmount = weaponData.healthChange;
	projectile->healthChange.source = GetOwner();
	projectile->healthChange.heals = weaponData.heals;
	return projectile;
}

void UWeapon::UseWeapon(const FVector& location)
{

}