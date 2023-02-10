#pragma once

#include "CoreMinimal.h"
#include "CSVTable.h"
#include "ArmourCSVDataTable.generated.h"

UCLASS()
class MECHRPG_API UArmourCSVDataTable : public UCSVTable
{
	GENERATED_BODY()
public:
	UArmourCSVDataTable();
	virtual void LoadData(TArray<TArray<FString>> inDataStrings) override;

	TArray<FArmourData >& GetData() { return armourData; }
private:
	TArray<FArmourData > armourData;
};
