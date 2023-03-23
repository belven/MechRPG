
#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Events/EventListener.h"
#include "BaseAIController.generated.h"

#define GameInstance(world) Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(world))
#define mAsMech(character) Cast<AMechRPGCharacter>(character)

class AMechRPGCharacter;
class IDamagable;
class UAISenseConfig_Sight;
class UEnvQuery;

UCLASS()
class MECHRPG_API ABaseAIController : public AAIController, public IEventListener
{
	GENERATED_BODY()
public:
	ABaseAIController();

	UFUNCTION()
		void PerceptionUpdated(const TArray<AActor*>& testActors);

	void MyQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	virtual void Tick(float DeltaTime) override;
	TArray<FHitResult> CheckLoSToTarget();
	bool CanSee(AActor* other, FVector startLoc);
	void FireShot(FVector FireDirection);
	void LookAt(FVector lookAtLocation);
	virtual void OnPossess(APawn* aPawn) override;
	virtual void EventTriggered(UBaseEvent* inEvent) override;

	IDamagable* GetTarget() { return  target; }
private:
	IDamagable* target;

	UPROPERTY()
		UAISenseConfig_Sight* sightConfig;

	UPROPERTY()
		UEnvQuery* MyQuery;

	UPROPERTY()
		FEnvQueryRequest MyQueryRequest;
};
