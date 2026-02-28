// Copyright Tarcisio Games

// Player controller detects pause request and forwards it here via the interface
// Pause Manager processes the request, pauses/unpauses and forwards the request to the UI Manager
// UI Manager shows/hides the pause menu
//
// TODO: Unpause when UI manager / pause menu wraps up its fade out?

#include "GJT_PauseManager.h"

void UGJT_PauseManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	bIsPaused = false;
	bCanPause = true;
}

bool UGJT_PauseManager::IsPaused() const
{
	return bIsPaused;
}

bool UGJT_PauseManager::RequestPauseStateChange_Implementation(bool bInPause)
{
	if (bInPause && !bCanPause)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("Pausing not allowed atm."));
#endif
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("PAUSE STATE %s"), bIsPaused ? TEXT("true") : TEXT("false"));

	bIsPaused = bInPause;

	GetWorld()->GetFirstPlayerController()->SetPause(bIsPaused);

	OnPauseStateChangedEvent.Broadcast(bIsPaused);

	return true;
}

bool UGJT_PauseManager::TogglePauseState_Implementation()
{
	return RequestPauseStateChange_Implementation(!bIsPaused);
}

void UGJT_PauseManager::SetCanPause_Implementation(bool bInCanPause)
{
	if (bCanPause == bInCanPause) { return; }

	// TODO: cannot pause if no gameplay scenes are opened
	// TODO: cannot pause during scene transitions

	bCanPause = bInCanPause;
}