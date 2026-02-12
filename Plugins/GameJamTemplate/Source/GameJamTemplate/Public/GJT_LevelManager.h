// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GJT_LevelManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGJTLevelTransitionSignature, TSoftObjectPtr<UWorld>, OldLevel, TSoftObjectPtr<UWorld>, NewLevel);

UCLASS()
class GAMEJAMTEMPLATE_API UGJT_LevelManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

protected:
    TSoftObjectPtr<UWorld> PreviousLevel;
    TSoftObjectPtr<UWorld> LoadingLevel;
    UPROPERTY()
    ULevelStreaming* LoadingLevelInstance;
    FLatentActionInfo SavedLatentInfo;
    bool bIsDoneLoading;

public:

#if WITH_EDITORONLY_DATA
    UPROPERTY()
    FSoftObjectPath EditorBootstrapMapPath;
#endif
    UPROPERTY(BlueprintAssignable, Category = "GJT | Navigation")
    FGJTLevelTransitionSignature OnBeforeLevelLoad;

    UPROPERTY(BlueprintAssignable, Category = "GJT | Navigation")
    FGJTLevelTransitionSignature OnAfterLevelLoad;

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintPure, Category = "GJT", meta = (WorldContext = "WorldContextObject"))
    TSoftObjectPtr<UWorld> GetCurrentLevelReference(const UObject* WorldContextObject);

    ULevelStreaming* GetCurrentLevelStreamingObject(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "GJT | Navigation")
    void LoadLevelByName(FName LevelName);

    UFUNCTION(BlueprintCallable, Category = "GJT | Navigation")
    void LoadLevelByReference(TSoftObjectPtr<UWorld> LevelRef);

    UFUNCTION(BlueprintCallable, Category = "GJT | Navigation", 
        meta = (Latent, LatentInfo = "LatentInfo"))
    void StreamLevelAsync(const UObject* WorldContextObject, TSoftObjectPtr<UWorld> LevelRef, FLatentActionInfo LatentInfo);
    
    UFUNCTION(BlueprintCallable)
    float GetStreamingProgress(TSoftObjectPtr<UWorld> LevelRef);

    UFUNCTION()
    void HandleLoadCompleted(const UObject* WorldContextObject, TSoftObjectPtr<UWorld> LevelRef);

    UFUNCTION()
    void OnLevelShownCallback();
};