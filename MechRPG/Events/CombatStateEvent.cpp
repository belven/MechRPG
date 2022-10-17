#include "CombatStateEvent.h"

UCombatStateEvent* UCombatStateEvent::CreateCombatStateEvent(IEventListener* owner, FCombatStateChange change)
{
	UCombatStateEvent* cse = NewObject<UCombatStateEvent>();
	cse->csc = change;
	cse->SetEventType(EEventType::CombatState);
	cse->SetEventOwner(owner);
	return cse;
}