#include "Damagable.h"

void IDamagable::ChangeHealth(FHealthChange& health_change)
{

}

float IDamagable::GetCurrentHealth()
{
	return 0;
}

float IDamagable::GetMaxHealth()
{
	return 0;
}

bool IDamagable::IsDead()
{
	return true;
}