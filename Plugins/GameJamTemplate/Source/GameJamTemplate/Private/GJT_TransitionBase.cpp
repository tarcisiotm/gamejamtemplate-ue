// Copyright Tarcisio Games


#include "GJT_TransitionBase.h"
#include "GJT_LevelManager.h"

void UGJT_TransitionBase::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* WorldPtr = GetWorld();
	if (!WorldPtr) { return; }

	UGJT_LevelManager* levelManager = WorldPtr->GetGameInstance()->GetSubsystem<UGJT_LevelManager>();
	if (!levelManager)
	{
		UE_LOG(LogTemp, Error, TEXT("Level Manager Subsystem not Found!!!"));
		return; 
	}

	//levelManager->OnBeforeLevelLoad
}

void UGJT_TransitionBase::FadeIn_Implementation()
{
}

void UGJT_TransitionBase::FadeOut_Implementation()
{
}

void UGJT_TransitionBase::BroadcastFadeOutFinished(EFadeType fadeType)
{
	{ OnFadeFinished.Broadcast(fadeType); }
}