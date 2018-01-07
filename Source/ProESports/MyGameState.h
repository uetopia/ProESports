// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "MyGameInstance.h"
//#include "MyServerPortalActor.h"
#include "MyGameState.generated.h"

/**
*
*/
UCLASS()
class PROESPORTS_API AMyGameState : public AGameState
{
	GENERATED_UCLASS_BODY()

		//UMyGameInstance gameInstance;

		virtual void BeginPlay();

	//void LoadLevel();

	/* Handle to manage the timer */
	FTimerHandle ServerPortalsTimerHandle;

	//void SpawnServerPortals();

public:

	UPROPERTY(Replicated, BlueprintReadOnly)
		FMyServerLinks ServerLinks;

	UPROPERTY(Replicated, BlueprintReadOnly)
		FString serverTitle;

	UPROPERTY(Replicated, BlueprintReadOnly)
		FString MatchTitle;

	UPROPERTY(Replicated, BlueprintReadOnly)
		FMyTeamList TeamList;
	//TArray<AMyServerPortalActor*> ServerPortalActorReference;
};
