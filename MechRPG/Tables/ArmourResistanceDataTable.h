#pragma once
#include "CoreMinimal.h"
#include "CSVTable.h"
#include "ArmourResistanceDataTable.generated.h"

UCLASS()
class MECHRPG_API UArmourResistanceDataTable : public UCSVTable
{
	GENERATED_BODY()
public:
	UArmourResistanceDataTable();
	virtual void LoadData(TArray<TArray<FString>> inDataStrings) override;

	TArray<FArmourResistanceData >& GetData() { return armourResistanceData; }
private:
	TArray<FArmourResistanceData > armourResistanceData;

};