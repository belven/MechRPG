#include "ItemDataTable.h"

UItemDataTable::UItemDataTable() : Super()
{
	path = CSVT::GetTableFilePath("ItemData.csv");
}

void UItemDataTable::LoadData(TArray<TArray<FString>> inDataStrings)
{
	for (TArray<FString> row : inDataStrings)
	{
		int index = 0;
		FItemData data;
		data.ID = GetIntFromString(row[index++]);
		data.name = *row[index++];
		data.type = DS::GetItemType(row[index++]);
		itemData.Add(data);
	}
}
