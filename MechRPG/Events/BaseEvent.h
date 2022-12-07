#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseEvent.generated.h"

class IEventListener;

UENUM(BlueprintType)
enum class  EEventType : uint8 {
	HealthChange,
	CombatState
};

UCLASS()
class MECHRPG_API UBaseEvent : public UObject
{
	GENERATED_BODY()
private:
	UObject* EventOwner;
	EEventType EventType;

public:
	EEventType GetEventType() const { return EventType; }
	void SetEventType(EEventType val) { EventType = val; }

	UObject* GetEventOwner() const { return EventOwner; }
	void SetEventOwner(UObject* eOwner) { EventOwner = eOwner; }
};