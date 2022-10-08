#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataStructs.h"
#include "CSVTable.generated.h"

#define DS UDataStructs
#define SourceFolder FPaths::GameSourceDir()
#define TablesFolder SourceFolder + "Tables/"
#define CSVT UCSVTable

UCLASS()
class MECHRPG_API UCSVTable : public UObject
{
	GENERATED_BODY()
public:
	virtual void LoadData(TArray<TArray<FString>> inDataStrings);
	FString GetPath() const { return path; }

	static FString GetTableFilePath(FString table)
	{
		FString path = TablesFolder + table;
		path.ReplaceCharInline(*"/", *"\\");
		//FPaths::NormalizeDirectoryName(path);
		return path;
	};

protected:
	TArray<TArray<FString>> dataStrings;
	FString path;

	int32 GetIntFromString(FString value) { return FCString::Atoi(*value);  }
	float GetFloatFromString(FString value) { return FCString::Atof(*value); }
};