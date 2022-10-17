#pragma once
#include "CoreMinimal.h"
#include "Tables/CSVTable.h"
#include "RPGGameInstance.generated.h"

#define GameInstance(world) Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(world))

class URPGEventManager;
class UProjectileWeaponDataTable;
class UMeleeWeaponDataTable;
class ULaserWeaponDataTable;
class URangedWeaponDataTable;
class UWeaponDataTable;
class UItemDataTable;

UCLASS()
class MECHRPG_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void LoadTableFromFile(UCSVTable* table);

	void LoadTableData();
	void LoadCombinedStructs();
	void LoadWeaponStructs();

	FItemData GetItemData(int32 itemID);
	FWeaponData GetWeaponData(int32 itemID);
	FMeleeWeaponData GetMeleeWeaponData(int32 weaponID);
	FRangedWeaponData GetRangedWeaponData(int32 weaponID);
	FProjectileWeaponData GetProjectileWeaponData(int32 rangedWeaponID);
	FLaserWeaponData GetLaserWeaponData(int32 rangedWeaponID);

	virtual void Init() override;
	URPGEventManager* GetEventManager();

	UItemDataTable* GetItemDataTable();
	UWeaponDataTable* GetWeaponDataTable();
	URangedWeaponDataTable* GetRangedWeaponData();
	ULaserWeaponDataTable* GetLaserWeaponData();
	UProjectileWeaponDataTable* GetProjectileWeaponData();
	UMeleeWeaponDataTable* GetMeleeWeaponData();

	TArray<FString> CleanData(TArray<FString> strings);
	template<class T> T* GetDataTable(T* value);
private:
	UPROPERTY()
		URPGEventManager* eventManager;

	UPROPERTY()
		UItemDataTable* ItemData;

	UPROPERTY()
		UWeaponDataTable* WeaponData;

	UPROPERTY()
		URangedWeaponDataTable* rangedWeaponData;

	UPROPERTY()
		ULaserWeaponDataTable* laserWeaponData;

	UPROPERTY()
		UMeleeWeaponDataTable* meleeWeaponData;

	UPROPERTY()
		UProjectileWeaponDataTable* projectileWeaponData;
};

template <class T>
T* URPGGameInstance::GetDataTable(T* value)
{
	if (value == NULL) {
		value = NewObject<T>(T::StaticClass());
	}

	return value;
}
