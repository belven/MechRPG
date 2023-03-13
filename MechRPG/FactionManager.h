#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Team.h"
#include "FactionManager.generated.h"

USTRUCT(BlueprintType)
struct FFactionRelationship
{
	GENERATED_USTRUCT_BODY()
public:
	FFactionRelationship()
	{
		faction = EFaction::Synths;
		relationship = ERelationshipType::Neutral;
	}

	FFactionRelationship(EFaction inFaction, ERelationshipType inRelationship)
	{
		faction = inFaction;
		relationship = inRelationship;
	}

	EFaction faction;
	ERelationshipType relationship;
};

UCLASS()
class MECHRPG_API UFactionManager : public UObject
{
	GENERATED_BODY()
public:
	UFactionManager();
	void CreateRelationship(EFaction factionA, EFaction factionB, ERelationshipType type);
	ERelationshipType GetRelationship(ITeam* teamA, ITeam* teamB);

private:
	TMap<EFaction, TArray<FFactionRelationship>> relationships;
};