#pragma once
#include "CoreMinimal.h"
#include "Damagable.h"
#include "Team.h"
#include "GameFramework/Character.h"
#include "Items/Weapon.h"
#include "MechRPGCharacter.generated.h"

class URPGGameInstance;
class UWeapon;
class UArmour;
class UBaseAbility;

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_USTRUCT_BODY()
public:
	float health;
	float moveSpeed;
};

UCLASS(Blueprintable)
class AMechRPGCharacter : public ACharacter, public IDamagable, public ITeam
{
	GENERATED_BODY()

public:
	AMechRPGCharacter();

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE UWeapon* GetEquippedWeapon() { return equippedWeapon; }
	FORCEINLINE void SetEquippedWeapon(UWeapon* weapon) { equippedWeapon = weapon; equippedWeapon->SetOwner(this); }

	URPGGameInstance* GetGameInstance();
	void EquipArmour(UArmour* armour);

	TArray<UBaseAbility*>& GetAbilities() { return abilities; }

	virtual void BeginPlay() override;

	// IDamagable Interface
	virtual void ChangeHealth(FHealthChange& health_change) override;
	virtual bool IsDead() override { return currentStats.health <= 0; };
	virtual float GetCurrentHealth() override { return currentStats.health; }
	virtual float GetMaxHealth() override { return maxStats.health; }
	virtual EFaction GetFaction() override { return faction; }
	void SetFaction(EFaction inFaction) { faction = inFaction; }
private:
	UPROPERTY()
		UWeapon* equippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		EFaction faction;

	UPROPERTY()
		TMap<EArmourSlot, UArmour*> equippedArmour;

	UPROPERTY()
		TArray<UBaseAbility*> abilities;

	UPROPERTY()
		URPGGameInstance* gameInstance;

	FCharacterStats currentStats;
	FCharacterStats maxStats;

	bool inCombat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	float GetDamageAfterResistance(float damage, EDamageType type);
	int32 GetDamageResistance(EDamageType type);
};