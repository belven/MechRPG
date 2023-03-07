#include "SingleTargetAbility.h"
#include "MechRPG/Damagable.h"

void USingleTargetAbility::Use(IDamagable* target)
{
	if (!IsOnCooldown()) {
		Super::Use(target);
		FHealthChange change;
		change.changeAmount = singleTargetData.healthChange;
		change.source = this->GetOwner();
		change.heals = singleTargetData.heals;
		change.damageType = singleTargetData.damageType;
		target->ChangeHealth(change);
	}
}