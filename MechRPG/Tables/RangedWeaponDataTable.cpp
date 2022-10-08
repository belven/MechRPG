#include "RangedWeaponDataTable.h"

URangedWeaponDataTable::URangedWeaponDataTable()
{
	path = CSVT::GetTableFilePath("RangedWeaponData.csv");
}

void URangedWeaponDataTable::LoadData(TArray<TArray<FString>> inDataStrings)
{
	for (TArray<FString> row : inDataStrings)
	{
		int index = 0;
		FRangedWeaponData data;
		data.ID = GetIntFromString(row[index++]);
		data.weaponID = GetIntFromString(row[index++]);
		data.accuracy = GetFloatFromString(row[index++]);
		rangedWeaponData.Add(data);
	}
}