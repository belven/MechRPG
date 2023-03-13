#include "Team.h"
#include "FactionManager.h"
#include "RPGGameInstance.h"

ERelationshipType ITeam::GetRelationship(ITeam* other, URPGGameInstance* instance)
{
	return instance->GetFactionManager()->GetRelationship(this, other);
}
