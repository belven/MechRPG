#include "RPGGameInstance.h"
#include "Tables/CSVTable.h"
#include "Tables/ItemDataTable.h"
#include "Tables/LaserWeaponDataTable.h"
#include "Tables/MeleeWeaponDataTable.h"
#include "Tables/ProjectileWeaponDataTable.h"
#include "Tables/RangedWeaponDataTable.h"
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
			parsedCSV.Add(CleanData(temp_array));
		}

		table->LoadData(parsedCSV);
	}
}

TArray<FString> URPGGameInstance::CleanData(TArray<FString> strings)
 {
	TArray<FString> newStrings;
	for(FString s : strings)
	{
		newStrings.Add(s.TrimQuotes());
	}
	return newStrings;
}

void URPGGameInstance::LoadTableData()
{
	LoadTableFromFile(GetItemDataTable());
	LoadTableFromFile(GetWeaponDataTable());
	LoadTableFromFile(GetRangedWeaponData());
	LoadTableFromFile(GetLaserWeaponData());
	LoadTableFromFile(GetMeleeWeaponData());

	LoadCombinedStructs();
}

void URPGGameInstance::LoadCombinedStructs()
{
	LoadWeaponStructs();
}

void URPGGameInstance::LoadWeaponStructs()
{
	TArray<FCombinedWeaponData> combinedWeaponData;
	TArray<FItemData> itemData = GetItemDataTable()->GetData();

	for (const FItemData id : itemData)
	{
		if (id.type == EItemType::Weapon) {
			FCombinedWeaponData cwd;
			cwd.itemData = id;
			cwd.weaponData = GetWeaponData(id.ID);

			if (cwd.weaponData.type == EWeaponType::Projectile)
			{
				cwd.rangedWeaponData = GetRangedWeaponData(cwd.weaponData.ID);
				cwd.projectileWeaponData = GetProjectileWeaponData(cwd.rangedWeaponData.ID);
			}
			else if (cwd.weaponData.type == EWeaponType::Laser) {
				cwd.rangedWeaponData = GetRangedWeaponData(cwd.weaponData.ID);
				cwd.laserWeaponData = GetLaserWeaponData(cwd.rangedWeaponData.ID);
			}
			else if (cwd.weaponData.type == EWeaponType::Melee) {
				cwd.meleeWeaponData = GetMeleeWeaponData(cwd.weaponData.ID);
			}

			combinedWeaponData.Add(cwd);
		}
	}
}

FItemData URPGGameInstance::GetItemData(int32 itemID)
{
	TArray<FItemData> itemData = GetItemDataTable()->GetData();
	for (const FItemData id : itemData)
	{
		if (id.ID == itemID)
			return id;
	}
	return {};
}

FWeaponData URPGGameInstance::GetWeaponData(int32 itemID)
{
	TArray<FWeaponData> weaponData = GetWeaponDataTable()->GetData();
	for (const FWeaponData wd : weaponData)
	{
		if (wd.itemID == itemID)
			return wd;
	}
	return {};
}

FMeleeWeaponData URPGGameInstance::GetMeleeWeaponData(int32 weaponID)
{
	for (const FMeleeWeaponData wd : GetMeleeWeaponData()->GetData())
	{
		if (wd.weaponID == weaponID)
			return wd;
	}
	return {};
}

FRangedWeaponData URPGGameInstance::GetRangedWeaponData(int32 weaponID)
{
	for (const FRangedWeaponData wd : GetRangedWeaponData()->GetData())
	{
		if (wd.weaponID == weaponID)
			return wd;
	}
	return {};
}

FProjectileWeaponData URPGGameInstance::GetProjectileWeaponData(int32 rangedWeaponID)
{
	for (const FProjectileWeaponData wd : GetProjectileWeaponData()->GetData())
	{
		if (wd.rangedWeaponID == rangedWeaponID)
			return wd;
	}
	return {};
}

FLaserWeaponData URPGGameInstance::GetLaserWeaponData(int32 rangedWeaponID)
{
	for (const FLaserWeaponData wd : GetLaserWeaponData()->GetData())
	{
		if (wd.rangedWeaponID == rangedWeaponID)
			return wd;
	}
	return {};
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
}

UWeaponDataTable* URPGGameInstance::GetWeaponDataTable()
{
	if (WeaponData == NULL)
	{
		WeaponData = NewObject<UWeaponDataTable>();
	}

	return WeaponData;
}

URangedWeaponDataTable* URPGGameInstance::GetRangedWeaponData()
{
	if (rangedWeaponData == NULL)
	{
		rangedWeaponData = NewObject<URangedWeaponDataTable>();
	}

	return rangedWeaponData;
}

ULaserWeaponDataTable* URPGGameInstance::GetLaserWeaponData()
{
	if (laserWeaponData == NULL)
	{
		laserWeaponData = NewObject<ULaserWeaponDataTable>();
	}

	return laserWeaponData;
}

UProjectileWeaponDataTable* URPGGameInstance::GetProjectileWeaponData()
{
	if (projectileWeaponData == NULL)
	{
		projectileWeaponData = NewObject<UProjectileWeaponDataTable>();
	}

	return projectileWeaponData;
}

UMeleeWeaponDataTable* URPGGameInstance::GetMeleeWeaponData()
{
	if (meleeWeaponData == NULL)
	{
		meleeWeaponData = NewObject<UMeleeWeaponDataTable>();
	}

	return meleeWeaponData;
}