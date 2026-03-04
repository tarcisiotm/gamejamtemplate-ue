// Copyright Tarcisio Games


#include "GJT_PauseMenu.h"

void UGJT_PauseMenu::OnPauseStateChanged_Implementation(bool IsPaused)
{
	if (IsPaused) { IGJT_WidgetInterface::Execute_Show(this); }
	else { IGJT_WidgetInterface::Execute_Hide(this); }
}


void UGJT_PauseMenu::Show_Implementation()
{
    Super::Show_Implementation();
    //SetVisibility(ESlateVisibility::Visible);

    // do fade
}

void UGJT_PauseMenu::Hide_Implementation()
{
    Super::Hide_Implementation();
}

void UGJT_PauseMenu::HandleTransitionDone_Implementation(bool bIsPaused)
{
    if (bIsPaused) { return; }
    
    SetVisibility(ESlateVisibility::Hidden);
}
