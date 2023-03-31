#pragma once
#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_WeaponLoS.generated.h"

UCLASS()
class MECHRPG_API UEnvQueryTest_WeaponLoS : public UEnvQueryTest
{
	GENERATED_BODY()
public:
	UEnvQueryTest_WeaponLoS(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = Trace)
		TSubclassOf<UEnvQueryContext> Context;

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
};
