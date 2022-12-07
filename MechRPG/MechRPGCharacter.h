#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Items/Weapon.h"
#include "MechRPGCharacter.generated.h"

class URPGGameInstance;
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


	URPGGameInstance* GetGameInstance();
private:
	UPROPERTY()
		UWeapon* equippedWeapon;

	UPROPERTY()
		URPGGameInstance* gameInstance;

	bool inCombat;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	float GetDamageAfterResistance(float damage, EDamageType type);
	int32 GetDamageResistance(EDamageType type);
};