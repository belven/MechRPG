#include "FindEnemyQueryContext.h"
#include "../BaseAIController.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

void UFindEnemyQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	const AActor* owner = Cast<AActor>((QueryInstance.Owner).Get());

	ABaseAIController* AICon = Cast<ABaseAIController>(owner->GetInstigatorController());

	if (AICon && AICon->GetTarget() != NULL)
	{
		UEnvQueryItemType_Point::SetContextHelper(ContextData, AICon->GetLastKnowLocation());
	}
}
