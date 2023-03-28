#pragma once
#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_WeaponDistance.generated.h"

UCLASS()
class MECHRPG_API UEnvQueryTest_WeaponDistance : public UEnvQueryTest
{
	GENERATED_BODY()
public:
	UEnvQueryTest_WeaponDistance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = Trace)
		TSubclassOf<UEnvQueryContext> Context;

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
};
