#include "PatrolPath.h"

#include "RPGGameInstance.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

APatrolPath::APatrolPath() : Super()
{
	spline = CreateDefaultSubobject<USplineComponent>(TEXT("Patrol Spline"));
	SetRootComponent(spline);
}

void APatrolPath::BeginPlay()
{
	Super::BeginPlay();
	GameInstance(GetWorld())->paths.AddUnique(this);
}