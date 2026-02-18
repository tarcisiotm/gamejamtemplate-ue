// Copyright Tarcisio Games


#include "GJT_AsyncLevelTransition.h"
#include "GJT_SubsystemHelperLibrary.h"

UGJT_AsyncLevelTransition* UGJT_AsyncLevelTransition::TransitionToLevelAsync(UObject* WorldContextObject, TSoftObjectPtr<UWorld> LevelRef, ESceneUnloadType UnloadType, bool bUsesTransition)
{
    UGJT_AsyncLevelTransition* Action = NewObject<UGJT_AsyncLevelTransition>();
    Action->WorldContext = WorldContextObject;
    Action->PendingLevel = LevelRef;
    Action->PendingUnloadType = UnloadType;
    Action->bPendingUsesTransition = bUsesTransition;
    return Action;
}

void UGJT_AsyncLevelTransition::Activate()
{
    auto LM = UGJT_SubsystemHelperLibrary::GetLevelManagerInterface(WorldContext);

    if (LM.GetInterface())
    {
        LM->GetOnAfterLevelLoadedEvent().AddDynamic(this, &UGJT_AsyncLevelTransition::HandleOnLevelLoadedBroadcast);

        IGJT_LevelManagerInterface::Execute_TransitionToLevel(
            LM.GetObject(),
            WorldContext,
            PendingLevel,
            PendingUnloadType,
            bPendingUsesTransition
        );
    }
    else
    {
        SetReadyToDestroy();
    }
}

void UGJT_AsyncLevelTransition::HandleOnLevelLoadedBroadcast(TSoftObjectPtr<UWorld> PreviousLevel, TSoftObjectPtr<UWorld> LoadedLevel)
{
    OnCompleted.Broadcast(PreviousLevel, LoadedLevel);
    SetReadyToDestroy();
}