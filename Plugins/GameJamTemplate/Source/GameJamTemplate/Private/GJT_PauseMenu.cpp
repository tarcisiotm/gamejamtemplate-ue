// Copyright Tarcisio Games


#include "GJT_PauseMenu.h"

void UGJT_PauseMenu::OnPauseStateChanged_Implementation(bool IsPaused)
{
	if (IsPaused) { Show(); }
	else { Hide(); }
}



void UGJT_PauseMenu::Show_Implementation()
{
    SetVisibility(ESlateVisibility::Visible);

    // do fade
}

void UGJT_PauseMenu::Hide_Implementation()
{

}

void UGJT_PauseMenu::HandleTransitionDone_Implementation(bool bIsPaused)
{
    if (bIsPaused) { return; }
    
    SetVisibility(ESlateVisibility::Hidden);
}
