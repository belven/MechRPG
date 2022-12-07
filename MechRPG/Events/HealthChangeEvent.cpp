#include "HealthChangeEvent.h"

UHealthChangeEvent* UHealthChangeEvent::CreateHealthChangeEvent(UObject* owner, FHealthChange hc, bool preHealthChange)
{
	UHealthChangeEvent* hce = NewObject<UHealthChangeEvent>();
	hce->hc = hc;
	hce->preHealthChange = preHealthChange;
	hce->SetEventType(EEventType::HealthChange);
	hce->SetEventOwner(owner);
	return hce;
}