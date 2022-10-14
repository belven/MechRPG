#include "WeaponCreator.h"
#include "LaserWeapon.h"
#include "MeleeWeapon.h"
#include "ProjectileWeapon.h"
#include "RangedWeapon.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "MechRPG/RPGGameInstance.h"
#include "../Tables/ItemDataTable.h"
#include "../Tables/LaserWeaponDataTable.h"
#include "../Tables/MeleeWeaponDataTable.h"
#include "../Tables/ProjectileWeaponDataTable.h"
#include "../Tables/RangedWeaponDataTable.h"
#include "../Tables/WeaponDataTable.h"

UWeapon* UWeaponCreator::CreateWeapon(const int32 itemID, const UWorld* world)
{
	UWeapon* weaponOut = NULL;
	URPGGameInstance* gameIn = GameInstance(world);
	const FItemData id = gameIn->GetItemData(itemID);

	if (id.type == EItemType::Weapon) {
		const FWeaponData w = gameIn->GetWeaponData(itemID);

		if (w.type == EWeaponType::Projectile)
		{
			UProjectileWeapon* pw = mNewObject(UProjectileWeapon);
			pw->SetItemData(id);
			pw->SetWeaponData(w);
			pw->SetRangedWeaponData(gameIn->GetRangedWeaponData(w.ID));
			pw->SetProjectileWeaponData(gameIn->GetProjectileWeaponData(pw->GetRangedWeaponData().ID));
			weaponOut = pw;
		}
		else	if (w.type == EWeaponType::Laser) {
			ULaserWeapon* lw = mNewObject(ULaserWeapon);
			lw->SetItemData(id);
			lw->SetWeaponData(w);
			lw->SetRangedWeaponData(gameIn->GetRangedWeaponData(w.ID));
			lw->SetLaserData(gameIn->GetLaserWeaponData(lw->GetRangedWeaponData().ID));
			weaponOut = lw;
		}
		else if (w.type == EWeaponType::Melee) {
			UMeleeWeapon* mw = mNewObject(UMeleeWeapon);
			mw->SetItemData(id);
			mw->SetWeaponData(w);
			mw->SetMeleeWeaponData(gameIn->GetMeleeWeaponData(w.ID));
			weaponOut = mw;
		}
	}
	return weaponOut;
}