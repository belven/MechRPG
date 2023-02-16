#include "LoadoutTableData.h"

ULoadoutTableData::ULoadoutTableData() : Super()
{
	path = CSVT::GetTableFilePath("LoadoutData.csv");
}

void ULoadoutTableData::LoadData(TArray<TArray<FString>> inDataStrings)
{
	for (TArray<FString> row : inDataStrings)
	{
		int index = 0;
		FLoadoutData data;
		data.ID = GetIntFromString(row[index++]);
		data.type = DS::GetCharacterType(row[index++]);
		data.entityID = GetIntFromString(row[index++]);
		data.weaponID = GetIntFromString(row[index++]);
		data.headArmourID = GetIntFromString(row[index++]);
		data.chestArmourID = GetIntFromString(row[index++]);
		data.leftLegArmourID = GetIntFromString(row[index++]);
		data.rightLegArmourID = GetIntFromString(row[index++]);
		data.leftArmArmourID = GetIntFromString(row[index++]);
		data.rightArmArmourID = GetIntFromString(row[index++]);
		data.abilityOneID = GetIntFromString(row[index++]);
		data.abilityTwoID = GetIntFromString(row[index++]);
		data.abilityThreeID = GetIntFromString(row[index++]);
		data.health = GetIntFromString(row[index++]);
		data.moveSpeed = GetIntFromString(row[index++]);
		loadoutData.Add(data);
	}
}