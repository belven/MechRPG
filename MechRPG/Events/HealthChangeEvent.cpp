#include "HealthChangeEvent.h"

UHealthChangeEvent* UHealthChangeEvent::CreateHealthChangeEvent(UObject* owner, FHealthChange hc, bool preHealthChange)
{
	UHealthChangeEvent* hce = NewObject<UHealthChangeEvent>();
	hce->hc = hc;
	hce->preHealthChange = preHealthChange;

	if (preHealthChange) {
		hce->SetEventType(EEventType::PreHealthChange );
	}
	else
	{
		hce->SetEventType(EEventType::PostHealthChange);		
	}

	hce->SetEventOwner(owner);
	return hce;
}