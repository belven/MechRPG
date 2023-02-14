#include "ArmourCreator.h"
#include "Armour.h"
#include "Kismet/GameplayStatics.h"
#include "MechRPG/RPGGameInstance.h"
#include "MechRPG/Tables/ArmourResistanceDataTable.h"

UArmour* UArmourCreator::CreateArmour(int32 itemID, UWorld* world)
{
	UArmour* a = NULL;
	URPGGameInstance* gameIn = GameInstance(world);
	const FItemData id = gameIn->GetItemData(itemID);

	const FArmourData ad = gameIn->GetArmourData(itemID);
	TArray<FArmourResistanceData> resistances;
	
	for(FArmourResistanceData ard : gameIn->GetArmourResistanceDataTable()->GetData())
	{
		if(ard.armourID == ad.ID)
		{
			resistances.Add(ard);
		}
	}

	a  = UArmour::CreateArmour(ad, resistances);
	a->SetItemData(id);

	return a;
}
