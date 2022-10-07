// Copyright Epic Games, Inc. All Rights Reserved.

#include "MechRPGGameMode.h"
#include "MechRPGPlayerController.h"
#include "MechRPGCharacter.h"
#include "TableLoader.h"
#include "UObject/ConstructorHelpers.h"

AMechRPGGameMode::AMechRPGGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMechRPGPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void AMechRPGGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMechRPGGameMode::BeginDestroy()
{
	Super::BeginDestroy();
	//UTableLoader::GetInstance()->BeginDestroy();
}
