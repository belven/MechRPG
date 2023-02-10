#include "ArmourResistanceDataTable.h"

UArmourResistanceDataTable::UArmourResistanceDataTable() : Super()
{
	path = CSVT::GetTableFilePath("ArmourResistanceData.csv");
}

void UArmourResistanceDataTable::LoadData(TArray<TArray<FString>> inDataStrings)
{
	for (TArray<FString> row : inDataStrings)
	{
		int index = 0;
		FArmourResistanceData data;
		data.ID = GetIntFromString(row[index++]);
		data.armourID = GetIntFromString(row[index++]);
		data.resistanceAmount = GetFloatFromString(row[index++]);
		data.type = DS::GetDamageType(row[index++]);
		armourResistanceData.Add(data);
	}
}