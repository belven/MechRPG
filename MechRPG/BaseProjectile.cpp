#include "BaseProjectile.h"
#include "MechRPGCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

const float ABaseProjectile::Default_Initial_Speed = 1000.0f;
const float ABaseProjectile::Default_Initial_Lifespan = 1.2f;

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'"));

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectileMesh->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
	ProjectileMesh->CastShadow = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = Default_Initial_Speed;
	ProjectileMovement->MaxSpeed = Default_Initial_Speed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity
	ProjectileMesh->SetWorldScale3D(FVector(0.4));
	RootComponent = ProjectileMesh;
	InitialLifeSpan = Default_Initial_Lifespan;
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != NULL && OtherActor != this && OtherActor->GetClass() != ABaseProjectile::StaticClass())
	{
		AMechRPGCharacter* mechHit = Cast<AMechRPGCharacter>(OtherActor);

		if(mechHit != NULL)
		{
			mechHit->ChangeHealth(healthChange);
		}
	}

	Destroy();
}
