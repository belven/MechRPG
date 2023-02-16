#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damagable.generated.h"

UINTERFACE(MinimalAPI)
class UDamagable : public UInterface
{
	GENERATED_BODY()
};

class MECHRPG_API IDamagable
{
	GENERATED_BODY()
		
public:
	virtual void ChangeHealth(FHealthChange& health_change);
	virtual float GetCurrentHealth();
	virtual float GetMaxHealth();
	bool IsAlive() { return !IsDead(); };
	virtual bool IsDead();
};