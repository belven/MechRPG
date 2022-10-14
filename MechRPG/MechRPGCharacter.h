#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Items/Weapon.h"
#include "MechRPGCharacter.generated.h"

class UWeapon;
UCLASS(Blueprintable)
class AMechRPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMechRPGCharacter();	
	
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE UWeapon* GetEquippedWeapon() { return equippedWeapon; }
	FORCEINLINE void SetEquippedWeapon(UWeapon* weapon) { equippedWeapon = weapon; equippedWeapon->SetOwner(this); }
	void ChangeHealth(const FHealthChange& health_change);
private:
	UPROPERTY()
		UWeapon* equippedWeapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
};