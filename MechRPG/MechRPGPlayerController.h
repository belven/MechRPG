#pragma once
#include "CoreMinimal.h"
#include "Events/EventListener.h"
#include "Templates/SubclassOf.h"
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
	virtual void OnPossess(APawn* aPawn) override;
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;

	virtual void BeginPlay() override;
	uint32 bMoveToMouseCursor : 1;

	virtual void PlayerTick(float DeltaTime) override;
	void LookAt(FVector lookAtLocation);
	void FireShot(FVector FireDirection);
	virtual void SetupInputComponent() override;
	void Fire();
	void StopFiring();
	void CalculateMovement(float DeltaSeconds) const;

	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
private:
	float MoveSpeed;

	bool bMovementActive;
	float FollowTime;
	float FiringTime;
	bool isFiring;
	FHitResult Hit;

	UPROPERTY()
		AMechRPGCharacter* mechTarget;
};