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
class UArmourCSVDataTable;
class UArmourResistanceDataTable;
class ULoadoutTableData;
class UFactionManager;
class APatrolPath;

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

	UFactionManager* GetFactionManager();

	FItemData GetItemData(int32 itemID);

	FWeaponData GetWeaponData(int32 itemID);
	FMeleeWeaponData GetMeleeWeaponData(int32 weaponID);
	FRangedWeaponData GetRangedWeaponData(int32 weaponID);
	FProjectileWeaponData GetProjectileWeaponData(int32 rangedWeaponID);
	FLaserWeaponData GetLaserWeaponData(int32 rangedWeaponID);

	FArmourData GetArmourData(int32 itemID);
	FArmourResistanceData GetArmourResistanceData(int32 armourID);

	FLoadoutData GetLoadoutData(int32 entityID);
	FAbilityData GetAbilityData(int32 abilityID);
	FSingleTargetAbilityData GetSingleTargetAbilityData(int32 abilityID);

	virtual void Init() override;
	URPGEventManager* GetEventManager();

	UItemDataTable* GetItemDataTable();
	UWeaponDataTable* GetWeaponDataTable();
	URangedWeaponDataTable* GetRangedWeaponData();
	ULaserWeaponDataTable* GetLaserWeaponData();
	UProjectileWeaponDataTable* GetProjectileWeaponData();
	UMeleeWeaponDataTable* GetMeleeWeaponData();
	UArmourCSVDataTable* GetArmourDataTable();
	UArmourResistanceDataTable* GetArmourResistanceDataTable();
	ULoadoutTableData* GetLoadoutTableData();

	static TArray<FString> CleanData(TArray<FString> strings);
	template<class T> T* GetDataTable(T* value);

	TArray<APatrolPath*> paths;
private:
	UPROPERTY()
		URPGEventManager* eventManager;

	UPROPERTY()
	UFactionManager* factionManager;

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

	UPROPERTY()
		UArmourCSVDataTable* armourDataTable;

	UPROPERTY()
		UArmourResistanceDataTable* armourResistanceDataTable;

	UPROPERTY()
		ULoadoutTableData* loadoutTableData;
};

template <class T>
T* URPGGameInstance::GetDataTable(T* value)
{
	if (value == NULL) {
		value = NewObject<T>();
	}

	return value;
}
