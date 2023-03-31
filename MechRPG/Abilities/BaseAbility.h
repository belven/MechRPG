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

	ABaseCharacter* GetOwner() { return owner; }
	void SetOwner(ABaseCharacter* inOwner) { owner = inOwner; }

protected:
	FTimerHandle TimerHandle_AbilityAvailable;
	FAbilityData data;
	bool onCooldown;

	UPROPERTY()
	ABaseCharacter* owner;
};
