#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseAbility.h"
#include "AbilityCreator.generated.h"

UCLASS()
class MECHRPG_API UAbilityCreator : public UObject
{
	GENERATED_BODY()
public:
		static UBaseAbility* CreateAbility(const int32 abilityID, const UWorld* world, AMechRPGCharacter* owner);

};
