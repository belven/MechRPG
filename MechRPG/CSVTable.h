#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataStructs.h"
#include "CSVTable.generated.h"

UCLASS()
class MECHRPG_API UCSVTable : public UObject
{
	GENERATED_BODY()
public:
	virtual void LoadData(TArray<TArray<FString>> inDataStrings);
	FString GetPath() const { return path; }
protected:
	TArray<TArray<FString>> dataStrings;
	FString path;

	int32 GetIntFromString(FString value) { return FCString::Atof(*value);  }
};