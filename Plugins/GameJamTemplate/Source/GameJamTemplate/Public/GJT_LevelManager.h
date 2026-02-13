// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/StreamableManager.h"
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

    bool bIsDoneLoading;

public:
    UPROPERTY(BlueprintReadWrite, Category = "Editor Only", meta = (DevelopmentOnly))
    FSoftObjectPath EditorBootstrapMapPath;

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

    UFUNCTION(BlueprintCallable, Category = "GJT | Navigation", meta = (Latent, LatentInfo = "LatentInfo", WorldContext = "WorldContextObject"))
    void StreamLevelAsync(const UObject* WorldContextObject, TSoftObjectPtr<UWorld> LevelRef, FLatentActionInfo LatentInfo);

    /** Returns 0.0-0.8 for Disk Load, 0.8-1.0 for World Initialization. */
    UFUNCTION(BlueprintCallable, Category = "GJT | Navigation")
    float GetStreamingProgress(TSoftObjectPtr<UWorld> LevelRef);

    UFUNCTION()
    void OnLevelShownCallback();
};