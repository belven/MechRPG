#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MechRPGGameMode.generated.h"

UCLASS(minimalapi)
class AMechRPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMechRPGGameMode();

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
};