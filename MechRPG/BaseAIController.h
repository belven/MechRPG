
#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "Events/EventListener.h"
#include "BaseAIController.generated.h"

#define GameInstance(world) Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(world))
#define mAsMech(character) Cast<AMechRPGCharacter>(character)

class AMechRPGCharacter;
class IDamagable;

UCLASS()
class MECHRPG_API ABaseAIController : public AAIController, public IEventListener
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	void FireShot(FVector FireDirection);
	void LookAt(FVector lookAtLocation);
	virtual void OnPossess(APawn* aPawn) override;
	virtual void EventTriggered(UBaseEvent* inEvent) override;
private:
	IDamagable* target;
};
