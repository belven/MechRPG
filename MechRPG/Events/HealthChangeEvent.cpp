#include "HealthChangeEvent.h"

UHealthChangeEvent* UHealthChangeEvent::CreateHealthChangeEvent(IEventListener* owner, FHealthChange hc, bool preHealthChange)
{
	UHealthChangeEvent* hce = NewObject<UHealthChangeEvent>();
	hce->hc = hc;
	hce->preHealthChange = preHealthChange;
	hce->SetEventType(EEventType::HealthChange);
	hce->SetEventOwner(owner);
	return hce;
}