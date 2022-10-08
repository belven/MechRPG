#pragma once
#include "CoreMinimal.h"
#include "CSVTable.h"
#include "ProjectileWeaponDataTable.generated.h"

UCLASS()
class MECHRPG_API UProjectileWeaponDataTable : public UCSVTable
{
	GENERATED_BODY()
public:
	UProjectileWeaponDataTable();
	virtual void LoadData(TArray<TArray<FString>> inDataStrings) override;

	TArray<FProjectileWeaponData >& GetData() { return weaponData; }
private:
	TArray<FProjectileWeaponData > weaponData;
};