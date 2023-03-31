#include "EnvQueryTest_WeaponLoS.h"
#include "../BaseAIController.h"
#include "../BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "Kismet/KismetSystemLibrary.h"

#define mSphereTraceMulti(start, end, radius, hits) UKismetSystemLibrary::SphereTraceMulti(GetWorld(), start, end, radius, ETraceTypeQuery::TraceTypeQuery1, true, ignore, EDrawDebugTrace::None, hits, true);

UEnvQueryTest_WeaponLoS::UEnvQueryTest_WeaponLoS(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set our ItemType to a vector, as the items (grid points) we're looking for, are locations in the world
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();

	// Set Filter Type to range, this will then be a curve of values which allows us to have scores from 0 - 1. I need to look into this more to understand it better
	FilterType = EEnvTestFilterType::Range;

	// If we're filtering only, then we can only pass or fail locations, and scoring only just manipulates the current scores of items.
	// We want to do both, as we need to filter out of range and out of line of sight items but also score in range and in line of sight items higher, the closer they are to our current location
	TestPurpose = EEnvTestPurpose::FilterAndScore;

	// Scores that are higher considered better
	ScoringEquation = EEnvTestScoreEquation::Linear;
}

void UEnvQueryTest_WeaponLoS::RunTest(FEnvQueryInstance& QueryInstance) const
{
	// Query Owner will be the controller that ran the query
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}

	TArray<FVector> ContextLocations;

	// This will get the locations created by our context, in our case it's a single location based on the AI Controllers GetLastKnowLocation
	if (!QueryInstance.PrepareContext(this->Context, ContextLocations))
	{
		return;
	}

	ABaseAIController* con = Cast<ABaseAIController>(QueryOwner);

	// Get the first and only location from the context
	FVector targetLocation = ContextLocations[0];

	// Get our AIs character, so we can get weapon range
	ABaseCharacter* controllerBaseCharacter = con->GetBaseCharacter();

	// Set up the ignore for the line trace
	TArray<AActor*> ignore;
	ignore.Add(con->GetCharacter());

	// Each Query will generate a list of Items, these items can either be FVectors or AActors
	// In our query, we made a grid search pattern, so we know these are going to be FVectors
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		// Get the current item as a FVector
		FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());

		// Check we have a weapon Equipped, this should always be the case
		if (controllerBaseCharacter->GetEquippedWeapon() != NULL) {
			float dist = FVector::Dist(targetLocation, ItemLocation);
			float range = controllerBaseCharacter->GetEquippedWeapon()->GetWeaponData().range;
			bool inRange = dist < range;

			// Check if the Item Location and our target location are within range of each other
			// item location is a single grid point and target is the AIs current target
			if (inRange) {
				TArray<FHitResult> hits;

				// Add some height to Sphere trace, otherwise it will collide with objects on the ground
				ItemLocation.Z += controllerBaseCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

				// Create a sphere trace, slightly larger than the characters capsule, so we make sure there's enough room to shoot
				mSphereTraceMulti(ItemLocation, targetLocation, con->GetCharacter()->GetCapsuleComponent()->GetScaledCapsuleRadius()* 1.5, hits);

				bool canSee = true;

				for(FHitResult hit : hits)
				{
					// Did we hit something?
					if(hit.bBlockingHit)
					{
						// If we hit something that's not are target FIRST, then there's something else in the way and we should invalidate that location
						if (hit.GetActor() != Cast<AActor>(con->GetTarget())) {
							canSee = false;
							break;
						}
						// If this is the case, we hit our target first, so stop checking line of sight
						else
						{
							break;
						}
					}
				}

				// Do we have clear Line of sight to the location?
				if (canSee)
				{
					// Create a score where closer locations to our AIs current location, are more valuable
					// This stops the AI going to the first location in the list, as sometimes it moves miles away to a valid location, even when there's one right next to it
					float score = 1 - (FVector::Dist(controllerBaseCharacter->GetActorLocation(), ItemLocation) / range);

					// Set the location as passed with a score
					It.ForceItemState(EEnvItemStatus::Passed, score);
				}
				// We don't have line of Sight to the target
				else
				{
					It.ForceItemState(EEnvItemStatus::Failed, 0);
				}
			}
			// Invalidate locations out of range locations
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