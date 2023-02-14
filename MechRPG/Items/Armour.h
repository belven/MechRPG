#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Armour.generated.h"


UCLASS()
class MECHRPG_API UArmour : public UItem
{
	GENERATED_BODY()
public:

	FArmourData GetData() const { return data; }
	void SetData(FArmourData inData) { this->data = inData; }

	TArray<FArmourResistanceData> GetResistances() const { return resistances; }
	void SetResistances(TArray<FArmourResistanceData> inResistances) { this->resistances = inResistances; }

	static UArmour* CreateArmour(FArmourData inData, TArray<FArmourResistanceData> inResistances);
private:
	FArmourData data;
	TArray<FArmourResistanceData> resistances;
};