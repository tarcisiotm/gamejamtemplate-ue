// Copyright Tarcisio Games


#include "GJT_PauseMenu.h"

void UGJT_PauseMenu::OnPauseStateChanged_Implementation(bool IsPaused)
{
	if (IsPaused) { HandleOnPaused_Implementation(); }
	else { HandleOnUnpaused_Implementation(); }
}

void UGJT_PauseMenu::HandleOnPaused_Implementation()
{
}

void UGJT_PauseMenu::HandleOnUnpaused_Implementation()
{
}
