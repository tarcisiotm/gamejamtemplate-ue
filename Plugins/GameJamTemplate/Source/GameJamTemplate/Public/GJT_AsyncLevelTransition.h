// Copyright Tarcisio Games


#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GJT_LevelManagerInterface.h"
#include "GJT_AsyncLevelTransition.generated.h"

UCLASS()
class GAMEJAMTEMPLATE_API UGJT_AsyncLevelTransition : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    F_GJT_OnLevelTransitionComplete OnCompleted;

    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "GJT | Navigation")
    static UGJT_AsyncLevelTransition* TransitionToLevelAsync(
        UObject* WorldContextObject,
        TSoftObjectPtr<UWorld> LevelRef,
        ESceneUnloadType UnloadType,
        bool bUsesTransition = true);

    virtual void Activate() override;

private:
    UObject* WorldContext;
    TSoftObjectPtr<UWorld> PendingLevel;
    ESceneUnloadType PendingUnloadType;
    bool bPendingUsesTransition;

    UFUNCTION()
    void HandleOnLevelLoadedBroadcast(TSoftObjectPtr<UWorld> PreviousLevel, TSoftObjectPtr<UWorld> LoadedLevel);
};