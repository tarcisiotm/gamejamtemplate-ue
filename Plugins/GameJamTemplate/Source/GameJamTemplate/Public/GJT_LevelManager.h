#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/LatentActionManager.h"
#include "GJT_Types.h"
#include "GJT_LevelManagerInterface.h"
#include "Tickable.h"
#include "GJT_LevelManager.generated.h"

class UGJT_TransitionBase;
class ULevelStreaming;

UENUM()
enum class ETransitionStage : uint8
{
    ShowingTransition,
    UnloadingBeforeNewScene,
    Loading,
    UnloadingAfterNewScene,
    HidingTransition,
    Finished
};


UCLASS()
class GAMEJAMTEMPLATE_API UGJT_LevelManager : public UGameInstanceSubsystem, public IGJT_LevelManagerInterface, public FTickableGameObject
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    // FTickableGameObject Interface
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UGJT_LevelManager, STATGROUP_Tickables); }

    //UFUNCTION(BlueprintCallable, Category = "GJT | Navigation", meta = (Latent, LatentInfo = "LatentInfo", WorldContext = "WorldContextObject"))
    virtual void TransitionToLevel_Implementation(
        const UObject* WorldContextObject,
        const TSoftObjectPtr<UWorld>& LevelRef,
        ESceneUnloadType UnloadType,
        //FLatentActionInfo LatentInfo,
        bool bUsesTransition) override;

    virtual F_GJT_OnLevelTransitionComplete& GetOnAfterLevelLoadedEvent() override { return OnAfterLevelLoad; }
    virtual F_GJT_OnFadeFinished& GetOnWidgetTransitionCompletedEvent() override { return OnTransitionFinished; }
    virtual F_GJT_OnLevelTransitionProgress& GetOnLevelTransitionProgressEvent() override { return OnLevelTransitionProgress; }

    UFUNCTION(BlueprintPure, Category = "GJT | Navigation")
    float GetCurrentTransitionProgress_Implementation() override;

    UPROPERTY(BlueprintAssignable, Category = "GJT | Events")
    F_GJT_OnLevelTransitionComplete OnAfterLevelLoad;

    UPROPERTY(BlueprintAssignable, Category = "GJT|Events")
    F_GJT_OnFadeFinished OnTransitionFinished;

    UPROPERTY(BlueprintAssignable, Category = "GJT|Events")
    F_GJT_OnLevelTransitionProgress OnLevelTransitionProgress;

    UPROPERTY(BlueprintReadOnly)
    FSoftObjectPath EditorBootstrapMapPath;

    UFUNCTION(BlueprintPure, Category = "GJT | Navigation", meta = (WorldContext = "WorldContextObject"))
    TSoftObjectPtr<UWorld> GetCurrentLevelReference(const UObject* WorldContextObject);



protected:
    // Internal Logic
    void UpdateCurrentLoadingStage(const UObject* WorldContextObject);
    void InternalLoad(const UObject* WorldContextObject);
    void InternalUnload(const UObject* WorldContextObject);

    void ShowTransitionWidget();
    void HideTransitionWidget();

    // Callbacks
    UFUNCTION() void OnLevelShownCallback();
    UFUNCTION() void OnLevelUnloadedCallback();

    // Helpers
    ULevelStreaming* GetCurrentLevelStreamingObject(const UObject* WorldContextObject);
    TSubclassOf<UGJT_TransitionBase> GetTransitionWidgetClass() const;
    UFUNCTION()
    void HandleWidgetFadeFinished(EFadeType FadeType);
    float GetStreamingProgress(TSoftObjectPtr<UWorld> LevelRef) const;

    // State Data
    ETransitionStage CurrentStage;
    ESceneUnloadType CurrentUnloadType;

    bool bUsesTransition;
    bool bWaitingForTransitionAnimation;
    bool bIsDoneUnloading;
    bool bIsDoneLoading;

    TSoftObjectPtr<UWorld> PreviousLevel;
    TSoftObjectPtr<UWorld> PendingLevel;
    TSoftObjectPtr<UWorld> LoadingLevel;

    UPROPERTY()
    TObjectPtr<UGJT_TransitionBase> ActiveTransitionWidget;
};