#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/LatentActionManager.h"
#include "GJT_Types.h"
#include "GJT_LevelManager.generated.h"

class UGJT_TransitionBase;
class ULevelStreaming;

UENUM(BlueprintType)
enum class ESceneUnloadType : uint8
{
    DoesNotUnload UMETA(DisplayName = "Does Not Unload"),
    BeforeNewSceneLoads UMETA(DisplayName = "Before New Scene Loads"),
    AfterNewSceneLoads  UMETA(DisplayName = "After New Scene Loads")
};

UENUM()
enum class ETransitionStage : uint8
{
    FadingOut,
    Unloading,
    Loading,
    FadingIn,
    Finished
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelTransitionSignature, TSoftObjectPtr<UWorld>, OldLevel, TSoftObjectPtr<UWorld>, NewLevel);

UCLASS()
class GAMEJAMTEMPLATE_API UGJT_LevelManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

    // This allows the Latent Action to see our protected variables
    friend class FGJT_LevelTransitionAction;

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category = "GJT | Navigation", meta = (Latent, LatentInfo = "LatentInfo", WorldContext = "WorldContextObject"))
    void TransitionToLevel(const UObject* WorldContextObject, TSoftObjectPtr<UWorld> LevelRef, ESceneUnloadType UnloadType, FLatentActionInfo LatentInfo);

    UFUNCTION(BlueprintPure, Category = "GJT | Navigation")
    float GetGlobalProgress() const;

    UPROPERTY(BlueprintAssignable, Category = "GJT | Events")
    FOnLevelTransitionSignature OnAfterLevelLoad;

    UPROPERTY(BlueprintReadOnly)
    FSoftObjectPath EditorBootstrapMapPath;

    // Add this wrapper so the old GameMode code still works without a transition
    UFUNCTION(BlueprintCallable, Category = "GJT | Navigation", meta = (Latent, LatentInfo = "LatentInfo", WorldContext = "WorldContextObject"))
    void LoadStreamLevelAsync(const UObject* WorldContextObject, TSoftObjectPtr<UWorld> LevelRef, bool bUsesTransition, FLatentActionInfo LatentInfo)
    {
        // If bUsesTransition is true, we use our new logic. 
        // If false, we use DoesNotUnload to match the old 'Async Load' behavior.
        TransitionToLevel(WorldContextObject, LevelRef, ESceneUnloadType::DoesNotUnload, LatentInfo);
    }

    UFUNCTION(BlueprintPure, Category = "GJT | Navigation", meta = (WorldContext = "WorldContextObject"))
    TSoftObjectPtr<UWorld> GetCurrentLevelReference(const UObject* WorldContextObject);

    UPROPERTY(BlueprintAssignable, Category = "GJT|Events")
    FOnFadeFinished OnTransitionFinished;

protected:
    // Internal Logic
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

    bool bIsFading;
    bool bIsUnloading;
    bool bIsDoneLoading;

    TSoftObjectPtr<UWorld> PreviousLevel;
    TSoftObjectPtr<UWorld> PendingLevel;
    TSoftObjectPtr<UWorld> LoadingLevel;

    UPROPERTY()
    TObjectPtr<UGJT_TransitionBase> ActiveTransitionWidget;
};