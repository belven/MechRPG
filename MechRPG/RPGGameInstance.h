#pragma once
#include "CoreMinimal.h"
#include "Tables/CSVTable.h"
#include "RPGGameInstance.generated.h"

class UItemDataTable;
/**
 * 
 */
UCLASS()
class MECHRPG_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
		UFUNCTION(BlueprintCallable)

	void LoadTableFromFile(UCSVTable* table);
	void LoadTableData();
	virtual void Init() override;

	UItemDataTable* GetItemData();
	
	virtual void BeginDestroy() override;
private:
	UItemDataTable* ItemData;
};
