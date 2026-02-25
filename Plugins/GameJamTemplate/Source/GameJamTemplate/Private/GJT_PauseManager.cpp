// Copyright Tarcisio Games


#include "GJT_PauseManager.h"

void UGJT_PauseManager::RequestPauseStateChange_Implementation(bool bInPause)
{
	if (bInPause && !bCanPause)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Display, TEXT("Pausing not allowed atm."));
#endif
		return;
	}

	bIsPaused = bInPause;

	GetWorld()->GetFirstPlayerController()->SetPause(bIsPaused);

	//OnPauseStateChangedDelegate.Broadcast(bIsPaused);
}

void UGJT_PauseManager::SetCanPause_Implementation(bool bInCanPause)
{
	if (bCanPause == bInCanPause) { return; }

	// TODO: cannot pause if no gameplay scenes are opened
	// TODO: cannot pause during scene transitions

	bCanPause = bInCanPause;
}