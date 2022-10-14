#pragma once
#include "CoreMinimal.h"
#include "Weapon.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class MECHRPG_API UMeleeWeapon : public UWeapon
{
	GENERATED_BODY()
public:
	FMeleeWeaponData GetMeleeWeaponData() const { return meleeWeaponData; }
	void SetMeleeWeaponData(FMeleeWeaponData data) { meleeWeaponData = data; }
protected:
	FMeleeWeaponData meleeWeaponData;
	
};