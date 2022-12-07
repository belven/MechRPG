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
		const bool isEventOwner = inEvent->GetEventOwner()->Implements<UEventListener>() && ei != Cast<IEventListener>(inEvent->GetEventOwner());
		if (inEvent->GetEventOwner() == NULL || !isEventOwner) {
			ei->EventTriggered(inEvent);
		}
	}
}