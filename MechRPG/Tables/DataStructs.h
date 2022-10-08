#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataStructs.generated.h"


USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY()
public:
	int32 ID;
	FString name;
};

UCLASS()
class MECHRPG_API UDataStructs : public UObject
{
	GENERATED_BODY()
	
};
