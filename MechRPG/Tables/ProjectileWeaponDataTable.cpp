#include "ProjectileWeaponDataTable.h"

UProjectileWeaponDataTable::UProjectileWeaponDataTable()
{
	path = CSVT::GetTableFilePath("ProjectileWeaponData.csv");
}

void UProjectileWeaponDataTable::LoadData(TArray<TArray<FString>> inDataStrings)
{
	for (TArray<FString> row : inDataStrings)
	{
		int index = 0;
		FProjectileWeaponData data;
		data.ID = GetIntFromString(row[index++]);
		data.rangedWeaponID = GetIntFromString(row[index++]);
		data.projectileClass = row[index++];
		data.magazineSize = GetIntFromString(row[index++]);
		data.reloadSpeed = GetFloatFromString(row[index++]);
		weaponData.Add(data);
	}
}