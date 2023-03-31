#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

class USplineComponent;
UCLASS()
class MECHRPG_API APatrolPath : public AActor
{
	GENERATED_BODY()
public:
	APatrolPath();
	
protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE USplineComponent* GetSpline() const { return spline; }
private:

	UPROPERTY(EditAnywhere)
		USplineComponent* spline;
};