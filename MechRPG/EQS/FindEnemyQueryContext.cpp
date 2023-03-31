#include "FindEnemyQueryContext.h"
#include "../BaseAIController.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

// This method is run every time a Query or Test, that uses this context, is run
void UFindEnemyQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	// The QueryInstance.Owner is the controller that performed the Query
	ABaseAIController* AICon = Cast<ABaseAIController>(QueryInstance.Owner.Get());

	// Make sure the owner is NULL and that we have a target set, otherwise this context is invalid
	if (AICon && AICon->GetTarget() != NULL)
	{
		// Add a single point i.e. FVector, to this context, as we're using LastKnowLocation and currently it's all we care about.
		// You can also add multiple points to a context to query multiple locations at the same time, say you wanted to check against any viable targets, rather than just the controllers current one
		UEnvQueryItemType_Point::SetContextHelper(ContextData, AICon->GetLastKnowLocation());
	}
}
