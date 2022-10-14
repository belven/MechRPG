#pragma once
#include "CoreMinimal.h"
#include "Item.h"
#include "MechRPG/Tables/DataStructs.h"
#include "Weapon.generated.h"

UCLASS()
class MECHRPG_API UWeapon : public UItem
{
	GENERATED_BODY()
public:
	FWeaponData GetWeaponData() const { return weaponData; }
	void SetWeaponData(FWeaponData data) { weaponData = data; }
protected:
	FWeaponData weaponData;

};
