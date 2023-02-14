#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ArmourCreator.generated.h"

class UArmour;

UCLASS()
class MECHRPG_API UArmourCreator : public UObject
{
	GENERATED_BODY()
public:
	static UArmour* CreateArmour(int32 itemID, UWorld* world);
	
};
