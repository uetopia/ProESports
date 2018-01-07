// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameState.h"
#include "Comp.h"
#include "Net/UnrealNetwork.h"
//#include "MyServerPortalActor.h"



AMyGameState::AMyGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameState] CONSTRUCT"));

	//GetWorld()->GetTimerManager().SetTimer(ServerPortalsTimerHandle, this, &AMyGameState::SpawnServerPortals, 20.0f, true);


}

void AMyGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyGameState, serverTitle);
	DOREPLIFETIME(AMyGameState, ServerLinks);
	DOREPLIFETIME(AMyGameState, TeamList);

}

void AMyGameState::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();


}

