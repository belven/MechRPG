#include "FactionManager.h"

UFactionManager::UFactionManager()
{
	CreateRelationship(EFaction::Synths, EFaction::Bandits, ERelationshipType::Enemy);
	CreateRelationship(EFaction::Synths, EFaction::Guardians, ERelationshipType::Ally);
	CreateRelationship(EFaction::Bandits, EFaction::Guardians, ERelationshipType::Neutral);	
}

void UFactionManager::CreateRelationship(EFaction factionA, EFaction factionB, ERelationshipType type)
{
	TArray<FFactionRelationship>& aRelationships = relationships.FindOrAdd(factionA);
	aRelationships.Add(FFactionRelationship(factionB, type));

	TArray<FFactionRelationship>& bRelationships = relationships.FindOrAdd(factionB);
	bRelationships.Add(FFactionRelationship(factionA, type));
}

ERelationshipType UFactionManager::GetRelationship(ITeam* teamA, ITeam* teamB)
{
	TArray<FFactionRelationship>& aRelationships = relationships.FindOrAdd(teamA->GetFaction());

	for(const FFactionRelationship fr : aRelationships)
	{
		if(fr.faction == teamB->GetFaction())
		{
			return fr.relationship;
		}
	}

	return ERelationshipType::Neutral;
}