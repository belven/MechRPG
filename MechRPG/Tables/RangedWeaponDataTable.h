#pragma once
#include "CoreMinimal.h"
#include "CSVTable.h"
#include "RangedWeaponDataTable.generated.h"

UCLASS()
class MECHRPG_API URangedWeaponDataTable : public UCSVTable
{
	GENERATED_BODY()
public:
	URangedWeaponDataTable();
	virtual void LoadData(TArray<TArray<FString>> inDataStrings) override;

	TArray<FRangedWeaponData >& GetData() { return rangedWeaponData; }
private:
	TArray<FRangedWeaponData > rangedWeaponData;
};