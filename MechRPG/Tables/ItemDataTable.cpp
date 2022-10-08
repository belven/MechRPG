#include "ItemDataTable.h"

UItemDataTable::UItemDataTable() : Super()
{
	path = CSVT::GetTableFilePath("ItemData.csv");
}

void UItemDataTable::LoadData(TArray<TArray<FString>> inDataStrings)
{
	for (TArray<FString> row : inDataStrings)
	{
		FItemData data;
		data.ID = GetIntFromString(row[0]);
		data.name = *row[1];
		itemData.Add(data);
	}
}
