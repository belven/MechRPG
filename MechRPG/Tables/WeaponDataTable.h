// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSVTable.h"
#include "WeaponDataTable.generated.h"

/**
 *
 */
UCLASS()
class MECHRPG_API UWeaponDataTable : public UCSVTable
{
	GENERATED_BODY()
public:
	UWeaponDataTable();
	virtual void LoadData(TArray<TArray<FString>> inDataStrings) override;

	TArray<FWeaponData >& GetWeaponData() { return weaponData; }
private:
	TArray<FWeaponData > weaponData;
};
