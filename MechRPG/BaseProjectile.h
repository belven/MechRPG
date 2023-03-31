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

	static const float Default_Initial_Speed;
	static const float Default_Initial_Lifespan;

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

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