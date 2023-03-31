#include "EnvQueryTest_WeaponLoS.h"
#include "../BaseAIController.h"
#include "../MechRPGCharacter.h"
#include "Components/CapsuleComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "Kismet/KismetSystemLibrary.h"

#define mSphereTraceMulti(start, end, radius, hits) UKismetSystemLibrary::SphereTraceMulti(GetWorld(), start, end, radius, ETraceTypeQuery::TraceTypeQuery1, true, ignore, EDrawDebugTrace::None, hits, true);

UEnvQueryTest_WeaponLoS::UEnvQueryTest_WeaponLoS(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
	FilterType = EEnvTestFilterType::Range;
	TestPurpose = EEnvTestPurpose::FilterAndScore;
	ScoringEquation = EEnvTestScoreEquation::InverseLinear;
}

void UEnvQueryTest_WeaponLoS::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}

	TArray<FVector> ContextLocations;

	if (!QueryInstance.PrepareContext(this->Context, ContextLocations))
	{
		return;
	}

	ABaseAIController* con = Cast<ABaseAIController>(QueryOwner);
	FVector targetLocation = ContextLocations[0];
	AMechRPGCharacter* mech = mAsMech(con->GetCharacter());

	TArray<AActor*> ignore;
	ignore.Add(con->GetCharacter());

	TArray<FHitResult> hits;

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
		
		if (mech->GetEquippedWeapon() != NULL) {
			float dist = FVector::Dist(targetLocation, ItemLocation);
			float range = mech->GetEquippedWeapon()->GetWeaponData().range;
			bool inRange = dist < range;

			if (inRange) {
				ItemLocation.Z += mech->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

				mSphereTraceMulti(ItemLocation, targetLocation, mAsMech(con->GetTarget())->GetCapsuleComponent()->GetScaledCapsuleRadius()* 1.5, hits);

				bool canSee = true;

				for(FHitResult hit : hits)
				{
					if(hit.bBlockingHit)
					{
						if (hit.GetActor() != Cast<AActor>(con->GetTarget())) {
							canSee = false;
							break;
						}
						else
						{
							break;
						}
					}
				}
				
				if (canSee)
				{
					float score = FVector::Dist(mech->GetActorLocation(), ItemLocation) / range;
					It.ForceItemState(EEnvItemStatus::Passed, score);
				}
				else
				{
					It.ForceItemState(EEnvItemStatus::Failed, 0);
				}
			}
			else
			{
				It.ForceItemState(EEnvItemStatus::Failed, 0);
			}
		}
	}
}

FText UEnvQueryTest_WeaponLoS::GetDescriptionTitle() const
{
	return FText::FromString(FString::Printf(TEXT("Weapon Distance and Line of Sight Test")));
}

FText UEnvQueryTest_WeaponLoS::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}