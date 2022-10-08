#include "MeleeWeaponDataTable.h"

UMeleeWeaponDataTable::UMeleeWeaponDataTable() : Super()
{
	path = CSVT::GetTableFilePath("MeleeWeaponData.csv");
}

void UMeleeWeaponDataTable::LoadData(TArray<TArray<FString>> inDataStrings)
{
	for (TArray<FString> row : inDataStrings)
	{
		int index = 0;
		FMeleeWeaponData data;
		data.ID = GetIntFromString(row[index++]);
		data.weaponID = GetIntFromString(row[index++]);
		data.cleaveRadius = GetFloatFromString(row[index++]);
		weaponData.Add(data);
	}
}