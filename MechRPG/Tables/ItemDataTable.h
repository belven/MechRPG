#pragma once
#include "CoreMinimal.h"
#include "CSVTable.h"
#include "ItemDataTable.generated.h"

UCLASS()
class MECHRPG_API UItemDataTable : public UCSVTable
{
	GENERATED_BODY()
public:
	UItemDataTable();
	virtual void LoadData(TArray<TArray<FString>> inDataStrings) override;
private:
	TArray<FItemData> itemData;
public:

	TArray<FItemData>& GetData() 
	{
		return itemData;
	}
};
