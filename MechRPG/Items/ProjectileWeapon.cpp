#include "ProjectileWeapon.h"

#include "MechRPG/BaseProjectile.h"

void UProjectileWeapon::UseWeapon(const FVector& location)
{
	if (canFire)
	{
		if (currentAmmo == 0)
		{
			canFire = false;
			mSetTimerWorld(owner->GetWorld(), TimerHandle_ShotTimerExpired, &UProjectileWeapon::ReloadEExpired, projectileWeaponData.reloadSpeed);
		}
		else
		{
			if (location.SizeSquared() > 0.0f)
			{
				const FRotator FireRotation = location.Rotation();
				const FVector gunLocation = owner->GetActorLocation() + FireRotation.RotateVector(GunOffset);

				ABaseProjectile* proj = SpawnProjectile(gunLocation, FireRotation, ABaseProjectile::StaticClass());

				mSetTimerWorld(owner->GetWorld(), TimerHandle_ShotTimerExpired, &UWeapon::ShotTimerExpired, GetWeaponData().useRate);
				currentAmmo--;
				canFire = false;
			}
		}
	}
}

void UProjectileWeapon::ReloadEExpired()
{
	canFire = true;
	currentAmmo = projectileWeaponData.magazineSize;
}
