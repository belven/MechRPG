#pragma once
#include "CoreMinimal.h"
#include "CSVTable.h"
#include "WeaponDataTable.generated.h"

UCLASS()
class MECHRPG_API UWeaponDataTable : public UCSVTable
{
	GENERATED_BODY()
public:
	UWeaponDataTable();
	virtual void LoadData(TArray<TArray<FString>> inDataStrings) override;

	TArray<FWeaponData >& GetData() { return weaponData; }
private:
	TArray<FWeaponData > weaponData;
};
