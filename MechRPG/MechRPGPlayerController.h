#pragma once
#include "CoreMinimal.h"
#include "Events/EventListener.h"
#include "GameFramework/PlayerController.h"
#include "MechRPGPlayerController.generated.h"

class AMechRPGCharacter;
class UNiagaraSystem;

UCLASS()
class AMechRPGPlayerController : public APlayerController, public IEventListener
{
	GENERATED_BODY()

public:
	AMechRPGPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		float ShortPressThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UNiagaraSystem* FXCursor;
	
	virtual void EventTriggered(UBaseEvent* inEvent) override;
protected:
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;	
	uint32 bMoveToMouseCursor : 1;

	virtual void PlayerTick(float DeltaTime) override;
	virtual void OnPossess(APawn* aPawn) override;

	void LookAt(FVector lookAtLocation);

	void FireShot(FVector FireDirection);
	void Fire();
	void StopFiring();

	virtual void SetupInputComponent() override;
	void CalculateMovement(float DeltaSeconds);
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	AMechRPGCharacter* GetMech() { return mech; }
private:
	float MoveSpeed;
	bool bMovementActive;
	float FollowTime;
	float FiringTime;
	bool isFiring;
	FHitResult Hit;

	UPROPERTY()
		AMechRPGCharacter* mech;

	UPROPERTY()
		AMechRPGCharacter* mechTarget;
};