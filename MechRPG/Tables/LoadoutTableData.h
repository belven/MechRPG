#pragma once
#include "CoreMinimal.h"
#include "CSVTable.h"
#include "LoadoutTableData.generated.h"

UCLASS()
class MECHRPG_API ULoadoutTableData : public UCSVTable
{
	GENERATED_BODY()
public:
	ULoadoutTableData();
	virtual void LoadData(TArray<TArray<FString>> inDataStrings) override;

	TArray<FLoadoutData >& GetData() { return loadoutData; }
private:
	TArray<FLoadoutData > loadoutData;
};
