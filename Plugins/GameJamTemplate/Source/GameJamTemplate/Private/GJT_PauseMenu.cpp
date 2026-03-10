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
}

void UGJT_PauseMenu::Hide_Implementation()
{
    Super::Hide_Implementation();
}
