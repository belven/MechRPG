// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "FindEnemyQueryContext.generated.h"

/**
 * 
 */
UCLASS()
class MECHRPG_API UFindEnemyQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()
public:
		virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};
