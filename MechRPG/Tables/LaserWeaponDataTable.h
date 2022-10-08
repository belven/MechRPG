// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSVTable.h"
#include "LaserWeaponDataTable.generated.h"

/**
 * 
 */
UCLASS()
class MECHRPG_API ULaserWeaponDataTable : public UCSVTable
{
	GENERATED_BODY()
public:
		ULaserWeaponDataTable();
	virtual void LoadData(TArray<TArray<FString>> inDataStrings) override;

	TArray<FLaserWeaponData >& GetLaserWeaponData() { return laserWeaponData; }
private:
	TArray<FLaserWeaponData > laserWeaponData;
};
