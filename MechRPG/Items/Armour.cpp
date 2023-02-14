#include "Armour.h"

UArmour* UArmour::CreateArmour(FArmourData inData, TArray<FArmourResistanceData> inResistances)
{
	UArmour* armour = NewObject<UArmour>();
	armour->SetData(inData);
	armour->SetResistances(inResistances);
	return armour;
}