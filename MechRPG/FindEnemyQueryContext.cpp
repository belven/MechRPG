#include "FindEnemyQueryContext.h"
#include "BaseAIController.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UFindEnemyQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	ABaseAIController* AICon = Cast<ABaseAIController>((Cast<AActor>((QueryInstance.Owner).Get())->GetInstigatorController()));

	if (AICon && AICon->GetTarget() != NULL)
	{
		//Set the context SeeingPawn to the provided context data
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(AICon->GetTarget()));
	}
}
