#include "AbilityCreator.h"
#include "SingleTargetAbility.h"
#include "MechRPG/RPGGameInstance.h"
#include "Kismet/GameplayStatics.h"

UBaseAbility* UAbilityCreator::CreateAbility(const int32 abilityID, const UWorld* world, AMechRPGCharacter* owner)
{
	UBaseAbility* ability = nullptr;
	URPGGameInstance* gameIn = GameInstance(world);

	const FAbilityData ad = gameIn->GetAbilityData(abilityID);

	switch (ad.type)
	{
	case EAbilityType::SingleTarget:
		USingleTargetAbility* sta = NewObject<USingleTargetAbility>();
		const FSingleTargetAbilityData stad = gameIn->GetSingleTargetAbilityData(abilityID);

		sta->SetAbilityData(ad);
		sta->SetSingleTargetAbilityData(stad);
		sta->SetOwner(owner);
		ability = sta;
		break;
	}
	return ability;
}
