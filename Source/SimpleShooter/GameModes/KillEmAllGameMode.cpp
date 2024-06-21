// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "SimpleShooter/Characters/ShooterAIController.h"

AKillEmAllGameMode::AKillEmAllGameMode()
{
}

void AKillEmAllGameMode::PawnKilled(APawn* KilledPawn)
{
	Super::PawnKilled(KilledPawn);

	APlayerController* PlayerController = Cast<APlayerController>(KilledPawn->GetController());

	if (PlayerController)
	{
		EndGame(false);
	}

	for (AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
	{
		if (!AIController->IsDead())
		{
			return;
		}
	}

	EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	// Use TActorRange to iterate through all actors of a certain class in the world
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		// bIsWinner is true only if (bIsPlayerWinner and the current Controller is the player) or
		// if (player is not the winner and the current controller is not the player)
		const bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
