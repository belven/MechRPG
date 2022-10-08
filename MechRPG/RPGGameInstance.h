#pragma once
#include "CoreMinimal.h"
#include "Tables/CSVTable.h"
#include "RPGGameInstance.generated.h"

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
	virtual void Init() override;

	UItemDataTable* GetItemDataTable();
	UWeaponDataTable* GetWeaponDataTable();

	virtual void BeginDestroy() override;

	template<class T> T* GetDataTable(T* value);
private:
	UPROPERTY()
		UItemDataTable* ItemData;
	UPROPERTY()
		UWeaponDataTable* WeaponData;
};

template <class T>
T* URPGGameInstance::GetDataTable(T* value)
{
	if (value == NULL) {
		value = NewObject<T>(T::StaticClass());
	}

	return value;
}
