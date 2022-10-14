#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponCreator.generated.h"

#define mNewObject(uClass) NewObject<uClass>() 
#define GameInstance(world) Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(world))

class UWeapon;
UCLASS()
class MECHRPG_API UWeaponCreator : public UObject
{
	GENERATED_BODY()
public:
		static UWeapon* CreateWeapon(const int32 itemID, const UWorld* world);
};