#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GJT_Types.h"
#include "GJT_LevelManagerInterface.h"
#include "Tickable.h"
#include "GJT_LevelManager.generated.h"

class IGJT_TransitionInterface;
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

    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UGJT_LevelManager, STATGROUP_Tickables); }

    virtual void TransitionToLevel_Implementation(
        const UObject* WorldContextObject,
        const TSoftObjectPtr<UWorld>& LevelRef,
        ESceneUnloadType UnloadType,
        bool bUsesTransition) override;

    virtual F_GJT_OnLevelTransitionComplete& GetOnAfterLevelLoadedEvent() override { return OnAfterLevelLoad; }
    virtual F_GJT_OnFadeFinished& GetOnWidgetTransitionCompletedEvent() override { return OnTransitionFinished; }
    virtual F_GJT_OnLevelTransitionProgress& GetOnLevelTransitionProgressEvent() override { return OnLevelTransitionProgress; }

    virtual FSoftObjectPath EditorOnly_GetEditorBootstrapMapPath_Implementation() const override
    {
#if WITH_EDITOR
        return EditorBootstrapMapPath;
#else
        return FSoftObjectPath();
#endif
    }

    virtual void EditorOnly_SetEditorBootstrapMapPath(FSoftObjectPath NewPath) override
    {
#if WITH_EDITOR
        EditorBootstrapMapPath = NewPath;
#endif
    }

    UFUNCTION(BlueprintPure, Category = "GJT | Navigation")
    float GetCurrentTransitionProgress_Implementation() override;

    UPROPERTY(BlueprintReadOnly)
    FSoftObjectPath EditorBootstrapMapPath;

    UFUNCTION(BlueprintPure, Category = "GJT | Navigation", meta = (WorldContext = "WorldContextObject"))
    TSoftObjectPtr<UWorld> GetCurrentLevelReference(const UObject* WorldContextObject);

protected:
    UPROPERTY(BlueprintAssignable, Category = "GJT | Events")
    F_GJT_OnLevelTransitionComplete OnAfterLevelLoad;

    UPROPERTY(BlueprintAssignable, Category = "GJT|Events")
    F_GJT_OnFadeFinished OnTransitionFinished;

    UPROPERTY(BlueprintAssignable, Category = "GJT|Events")
    F_GJT_OnLevelTransitionProgress OnLevelTransitionProgress;

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
    TObjectPtr<UUserWidget> ActiveTransitionWidget;

    UPROPERTY()
    TScriptInterface<IGJT_TransitionInterface> TransitionWidgetInterface;

    ULevelStreaming* GetCurrentLevelStreamingObject(const UObject* WorldContextObject);
    TSubclassOf<UUserWidget> GetTransitionWidgetClass() const;

    void UpdateCurrentLoadingStage(const UObject* WorldContextObject);
    void InternalLoad(const UObject* WorldContextObject);
    void InternalUnload(const UObject* WorldContextObject);

    void ShowTransitionWidget();
    void HideTransitionWidget();

    UFUNCTION() 
    void OnLevelShownCallback();

    UFUNCTION()
    void OnLevelUnloadedCallback();

    UFUNCTION()
    void HandleWidgetFadeFinished(EFadeType FadeType);
    float GetStreamingProgress(TSoftObjectPtr<UWorld> LevelRef) const;

};
