#include "ArmourCSVDataTable.h"

UArmourCSVDataTable::UArmourCSVDataTable() : Super()
{
	path = "D:\\UE5\\MechRPG\\Source\\Tables\\ArmourData.csv";
}

void UArmourCSVDataTable::LoadData(TArray<TArray<FString>> inDataStrings)
{
	for (TArray<FString> row : inDataStrings)
	{
		int index = 0;
		FArmourData data;
		data.ID = GetIntFromString(row[index++]);
		data.itemID = GetIntFromString(row[index++]);
		data.slot = DS::GetArmourSlot(row[index++]);
		armourData.Add(data);
	}
}