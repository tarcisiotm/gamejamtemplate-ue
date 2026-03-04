// Copyright Tarcisio Games


#include "GJT_GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

EGJT_GameContext UGJT_GameInstance::GetGameContext_Implementation() const
{
    return CurrentGameContext;
}