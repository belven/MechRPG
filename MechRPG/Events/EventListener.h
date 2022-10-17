#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EventListener.generated.h"

class UBaseEvent;
UINTERFACE(MinimalAPI)
class UEventListener : public UInterface
{
	GENERATED_BODY()
};

class MECHRPG_API IEventListener
{
	GENERATED_BODY()
		
public:
	virtual void EventTriggered(UBaseEvent* inEvent);
};