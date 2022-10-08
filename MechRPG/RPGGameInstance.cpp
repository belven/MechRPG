#include "RPGGameInstance.h"
#include "Tables/CSVTable.h"
#include "Tables/ItemDataTable.h"
#include "Tables/WeaponDataTable.h"

void URPGGameInstance::LoadTableFromFile(UCSVTable* table)
{
	TArray<TArray<FString>> parsedCSV;
	const FString path = table->GetPath();

	if (FPaths::FileExists(path))
	{
		FString FileContent;

		FFileHelper::LoadFileToString(FileContent, *path);

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
	LoadTableFromFile(GetItemDataTable());
	LoadTableFromFile(GetWeaponDataTable());

	TArray<FCombinedWeaponData> combinedWeaponData;
	TArray<FItemData> itemData = GetItemDataTable()->GetItemData();
	TArray<FWeaponData> weaponData = GetWeaponDataTable()->GetWeaponData();

	for (FItemData id : itemData)
	{
		FCombinedWeaponData cwd;
		cwd.itemData = id;

		for (FWeaponData wd : weaponData)
		{
			if (wd.itemID == id.ID)
			{
				cwd.weaponData = wd;
			}
		}

		combinedWeaponData.Add(cwd);
	}
}

void URPGGameInstance::Init()
{
	Super::Init();
	LoadTableData();
}

UItemDataTable* URPGGameInstance::GetItemDataTable()
{
	if (ItemData == NULL)
	{
		ItemData = NewObject<UItemDataTable>();
	}

	return ItemData;

	//return  GetDataTable<UItemDataTable>(ItemData);
}

UWeaponDataTable* URPGGameInstance::GetWeaponDataTable()
{
	if (WeaponData == NULL)
	{
		WeaponData = NewObject<UWeaponDataTable>();
	}

	return WeaponData;
//	return  GetDataTable<UWeaponDataTable>(WeaponData);
}

void URPGGameInstance::BeginDestroy()
{
	UObject::BeginDestroy();
	//ItemData->BeginDestroy();
}
