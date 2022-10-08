#include "LaserWeaponDataTable.h"

ULaserWeaponDataTable::ULaserWeaponDataTable() : Super()
{
	path = "D:\\UE5\\MechRPG\\Source\\Tables\\LaserWeaponData.csv";
}

void ULaserWeaponDataTable::LoadData(TArray<TArray<FString>> inDataStrings)
{
	for (TArray<FString> row : inDataStrings)
	{
		FLaserWeaponData data;
		data.ID = GetIntFromString(row[0]);
		data.rangedWeaponID = GetIntFromString(row[1]);
		data.particleEffect = row[2];
		laserWeaponData.Add(data);
	}
}
