
#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Events/EventListener.h"
#include "Perception/AIPerceptionTypes.h"
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
		void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	void MyQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	virtual void Tick(float DeltaTime) override;
	TArray<FHitResult> CheckLoSToTarget();
	void FireShot(FVector FireDirection);
	void LookAt(FVector lookAtLocation);
	virtual void OnPossess(APawn* aPawn) override;
	virtual void EventTriggered(UBaseEvent* inEvent) override;
	virtual void BeginPlay() override;

	FVector GetLastKnowLocation() { return lastKnowLocation;  }

	IDamagable* GetTarget() { return  target; }
private:
	IDamagable* target;
	FVector lastKnowLocation;
	bool canSee = false;

	UPROPERTY()
		UAISenseConfig_Sight* sightConfig;

	UPROPERTY()
		UEnvQuery* MyQuery;

	UPROPERTY()
		FEnvQueryRequest MyQueryRequest;
};
