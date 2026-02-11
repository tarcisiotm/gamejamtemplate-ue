// Copyright Tarcisio Games


#include "GJT_GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UGJT_GameInstance::TransitionToLevel(FName LevelName)
{
    if (TransitionWidgetClass)
    {
        ActiveTransitionWidget = CreateWidget<UUserWidget>(this, TransitionWidgetClass);
        ActiveTransitionWidget->AddToViewport(999); // Highest Z-order
    }

    // Logic for loading goes here (Simple OpenLevel or Async)
    UGameplayStatics::OpenLevel(this, LevelName);
}

void UGJT_GameInstance::ReportManagerReady(FName ManagerName)
{
    PendingManagers.Remove(ManagerName);
    CheckAllManagersReady();
}

void UGJT_GameInstance::CheckAllManagersReady()
{
    if (PendingManagers.Num() == 0 && ActiveTransitionWidget)
    {
        // Play Fade Out animation or Remove from parent
        ActiveTransitionWidget->RemoveFromParent();
    }
}