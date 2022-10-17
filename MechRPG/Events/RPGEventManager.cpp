#include "RPGEventManager.h"
#include "BaseEvent.h"

void URPGEventManager::RegisterListener(TArray<EEventType> types, IEventListener* listener)
{
	for (EEventType type : types) {		
		TArray<IEventListener*>& eventListeners = GetManagerListeners().FindOrAdd(type).listeners;
		eventListeners.Add(listener);
	}
}

void URPGEventManager::EventTriggered(UBaseEvent* inEvent)
{
	TArray<IEventListener*>& eventListeners = GetManagerListeners().Find(inEvent->GetEventType())->listeners;

	for (IEventListener* ei : eventListeners)
	{
		if (inEvent->GetEventOwner() == NULL || ei != inEvent->GetEventOwner())
			ei->EventTriggered(inEvent);
	}
}