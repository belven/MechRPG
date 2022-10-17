#pragma once
#include "CoreMinimal.h"
#include "BaseEvent.h"
#include "EventListener.h"
#include "UObject/NoExportTypes.h"
#include "RPGEventManager.generated.h"

#define mEventTriggered(GameInstance, baseEvent)  GameInstance->GetEventManager()->EventTriggered(baseEvent)

USTRUCT(BlueprintType)
struct FListeners 
{
	GENERATED_USTRUCT_BODY()
public:
		TArray<IEventListener*>  listeners;
};

UCLASS()
class MECHRPG_API URPGEventManager : public UObject
{
	GENERATED_BODY()

public:
		TMap<EEventType, FListeners>& GetManagerListeners()  { return ManagerListeners; }

		void RegisterListener(TArray<EEventType> types, IEventListener* listener);
		void EventTriggered(UBaseEvent* inEvent);
private:
	TMap< EEventType, FListeners> ManagerListeners;
	
};
