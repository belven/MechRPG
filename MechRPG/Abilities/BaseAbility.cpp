#include "BaseAbility.h"
#include "MechRPG/MechRPGCharacter.h"

UBaseAbility::UBaseAbility() : Super()
{
	onCooldown = false;
}

void UBaseAbility::Use(IDamagable* target)
{
	if (!onCooldown) {
		onCooldown = true;
		Cast<AActor>(target)->GetWorld()->GetTimerManager().SetTimer(TimerHandle_AbilityAvailable, this, &UBaseAbility::CooldownEnded, GetAbilityData().cooldown);
	}
}

void UBaseAbility::CooldownEnded()
{
	onCooldown = false;
}
