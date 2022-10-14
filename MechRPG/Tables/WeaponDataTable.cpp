#include "WeaponDataTable.h"

UWeaponDataTable::UWeaponDataTable()
{
	path = CSVT::GetTableFilePath("WeaponData.csv");
}

void UWeaponDataTable::LoadData(TArray<TArray<FString>> inDataStrings)
{
	for (TArray<FString> row : inDataStrings)
	{
		int index = 0;
		FWeaponData data;
		data.ID = GetIntFromString(row[index++]);
		data.itemID = GetIntFromString(row[index++]);
		data.type = DS::GetWeaponType(row[index++]);
		data.range = GetFloatFromString(row[index++]);
		data.healthChange = GetFloatFromString(row[index++]);
		data.useRate = GetFloatFromString(row[index++]);
		data.heals = DS::GetBoolean(row[index++]);
		weaponData.Add(data);
	}
}