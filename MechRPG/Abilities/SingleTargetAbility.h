#pragma once
#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "SingleTargetAbility.generated.h"

UCLASS()
class MECHRPG_API USingleTargetAbility : public UBaseAbility
{
	GENERATED_BODY()
public:
	virtual void Use(IDamagable* target) override;

	void SetSingleTargetAbilityData(FSingleTargetAbilityData inData) { singleTargetData = inData; }
private:
	FSingleTargetAbilityData singleTargetData;

};
