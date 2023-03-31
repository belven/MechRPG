#pragma once
#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class MECHRPG_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
public:
	ABaseProjectile();

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	static const float Default_Initial_Speed;
	static const float Default_Initial_Lifespan;

	void SetHealthChange(FHealthChange inHealthChange);

protected:
	FHealthChange healthChange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY()
		USphereComponent* CollisionComp;
};