#pragma once
#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "MechRPGPlayerController.generated.h"

class AMechRPGCharacter;
class UNiagaraSystem;

UCLASS()
class AMechRPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMechRPGPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		float ShortPressThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UNiagaraSystem* FXCursor;
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

	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
private:
	bool bMovementActive;
	float FollowTime;
	float FiringTime;
	bool isFiring;
	FHitResult Hit;

	UPROPERTY()
		AMechRPGCharacter* mechTarget;
};