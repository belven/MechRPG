#include "BaseProjectile.h"
#include "MechRPGCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

const float ABaseProjectile::Default_Initial_Speed = 1000.0f;
const float ABaseProjectile::Default_Initial_Lifespan = 1.2f;

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(50.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'"));

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->CastShadow = false;
	ProjectileMesh->SetWorldScale3D(FVector(0.05));
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = Default_Initial_Speed;
	ProjectileMovement->MaxSpeed = Default_Initial_Speed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMesh->SetWorldScale3D(FVector(0.2));
	InitialLifeSpan = Default_Initial_Lifespan;
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AMechRPGCharacter* us = healthChange.source;

	if (OtherActor != NULL && OtherActor != this && OtherActor != us)
	{
		if (OtherActor->Implements<UDamagable>())
		{
			ITeam* hitTeam = Cast<ITeam>(OtherActor);

			if (hitTeam->GetRelationship(us, us->GetGameInstance()) == ERelationshipType::Enemy) {
				IDamagable* hit = Cast<IDamagable>(OtherActor);
				hit->ChangeHealth(healthChange);
				Destroy();
			}
		}
		else
		{
			Destroy();
		}
	}
}

void ABaseProjectile::SetHealthChange(FHealthChange inHealthChange)
{
	this->healthChange = inHealthChange;
	CollisionComp->IgnoreActorWhenMoving(inHealthChange.source, true);
}