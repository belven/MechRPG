#pragma once
#include "CoreMinimal.h"
#include "MechRPG/Tables/DataStructs.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

UCLASS()
class MECHRPG_API UItem : public UObject
{
	GENERATED_BODY()
public:
	FItemData GetItemData() const { return itemData; }
	void SetItemData(FItemData data) { itemData = data; }
protected:
	FItemData itemData;
};