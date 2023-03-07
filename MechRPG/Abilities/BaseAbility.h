#pragma once
#include "CoreMinimal.h"
#include "MechRPG/Tables/DataStructs.h"
#include "UObject/NoExportTypes.h"
#include "../Damagable.h"
#include "BaseAbility.generated.h"

UCLASS()
class MECHRPG_API UBaseAbility : public UObject
{
	GENERATED_BODY()
public:
	UBaseAbility();

	virtual void Use(IDamagable* target);

	FAbilityData GetAbilityData() { return data;  }
	void SetAbilityData(FAbilityData inData) { data = inData; }

	void CooldownEnded();
	bool IsOnCooldown() { return onCooldown;  }

	AMechRPGCharacter* GetOwner() { return owner; }
	void SetOwner(AMechRPGCharacter* inOwner) { owner = inOwner; }

protected:
	FTimerHandle TimerHandle_AbilityAvailable;
	FAbilityData data;
	bool onCooldown;

	UPROPERTY()
	AMechRPGCharacter* owner;
};
