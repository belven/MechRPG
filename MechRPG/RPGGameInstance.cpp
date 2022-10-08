#include "RPGGameInstance.h"
#include "Tables/CSVTable.h"
#include "Tables/ItemDataTable.h"

void URPGGameInstance::LoadTableFromFile(UCSVTable* table)
{
	TArray<TArray<FString>> parsedCSV;

	if (FPaths::FileExists(table->GetPath()))
	{
		FString FileContent;

		FFileHelper::LoadFileToString(FileContent, *table->GetPath());

		const TCHAR* Terminators[] = { L"\r", L"\n" };
		const TCHAR* CSVDelimiters[] = { TEXT(","), TEXT("\t") };

		TArray<FString> CSVLines;
		FileContent.ParseIntoArray(CSVLines, Terminators, 2);

		TArray<FString> temp_array;
		for (int i = 0; i < CSVLines.Num(); i++) {
			temp_array.Empty();
			CSVLines[i].ParseIntoArray(temp_array, CSVDelimiters, 2);
			parsedCSV.Add(temp_array);
		}

		table->LoadData(parsedCSV);
	}
}

void URPGGameInstance::LoadTableData()
{
	LoadTableFromFile(GetItemData());
}

void URPGGameInstance::Init()
{
	Super::Init();
	LoadTableData();
}

UItemDataTable* URPGGameInstance::GetItemData()
{
	if (ItemData == NULL)
	{
		ItemData = NewObject<UItemDataTable>();
	}

	return ItemData;
}

void URPGGameInstance::BeginDestroy()
{
	UObject::BeginDestroy();
	//ItemData->BeginDestroy();
}