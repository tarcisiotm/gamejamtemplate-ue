// Copyright Tarcisio Games

#include "GJT_LevelManager.h"
#include "Kismet/GameplayStatics.h"
#include "LatentActions.h"
#include "Engine/LevelStreaming.h"
#include "Engine/Engine.h"
#include "Engine/AssetManager.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

class FGJT_LevelTransitionAction : public FPendingLatentAction
{
public:
    bool* bInternalLoadFinished;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

    FGJT_LevelTransitionAction(bool* InBool, const FLatentActionInfo& LatentInfo)
        : bInternalLoadFinished(InBool)
        , ExecutionFunction(LatentInfo.ExecutionFunction)
        , OutputLink(LatentInfo.Linkage)
        , CallbackTarget(LatentInfo.CallbackTarget)
    {
    }

    virtual void UpdateOperation(FLatentResponse& Response) override
    {
        Response.FinishAndTriggerIf(*bInternalLoadFinished, ExecutionFunction, OutputLink, CallbackTarget);
    }
};

void UGJT_LevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    bIsDoneLoading = true;
}

ULevelStreaming* UGJT_LevelManager::GetCurrentLevelStreamingObject(const UObject* WorldContextObject)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World) return nullptr;

    for (ULevelStreaming* Level : World->GetStreamingLevels())
    {
        if (Level && Level->IsLevelVisible())
        {
            return Level;
        }
    }
    return nullptr;
}

TSoftObjectPtr<UWorld> UGJT_LevelManager::GetCurrentLevelReference(const UObject* WorldContextObject)
{
    ULevelStreaming* LevelObj = GetCurrentLevelStreamingObject(WorldContextObject);
    if (LevelObj)
    {
        return TSoftObjectPtr<UWorld>(LevelObj->GetWorldAsset().ToSoftObjectPath());
    }
    return nullptr;
}

void UGJT_LevelManager::LoadLevelByName(FName LevelName)
{
    UWorld* World = GetWorld();
    if (!World || LevelName.IsNone()) { return; }
    UGameplayStatics::OpenLevel(World, LevelName);
}

void UGJT_LevelManager::LoadLevelByReference(TSoftObjectPtr<UWorld> LevelRef)
{
    UWorld* World = GetWorld();
    if (!World || LevelRef.IsNull()) { return; }
    UGameplayStatics::OpenLevelBySoftObjectPtr(World, LevelRef);
}

void UGJT_LevelManager::StreamLevelAsync(const UObject* WorldContextObject, TSoftObjectPtr<UWorld> LevelRef, FLatentActionInfo LatentInfo)
{
    if (LevelRef.IsNull()) return;
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World) return;

    bIsDoneLoading = false;
    LoadingLevel = LevelRef;

    UGameplayStatics::LoadStreamLevelBySoftObjectPtr(World, LevelRef, true, false, LatentInfo);

    FString TargetPackage = LevelRef.ToSoftObjectPath().GetLongPackageName();
    for (ULevelStreaming* Streaming : World->GetStreamingLevels())
    {
        if (Streaming && Streaming->GetWorldAssetPackageName().Equals(TargetPackage, ESearchCase::IgnoreCase))
        {
            Streaming->OnLevelShown.RemoveDynamic(this, &UGJT_LevelManager::OnLevelShownCallback);
            Streaming->OnLevelShown.AddDynamic(this, &UGJT_LevelManager::OnLevelShownCallback);
            break;
        }
    }

    FLatentActionManager& LatentManager = World->GetLatentActionManager();
    LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
        new FGJT_LevelTransitionAction(&bIsDoneLoading, LatentInfo));
}

float UGJT_LevelManager::GetStreamingProgress(TSoftObjectPtr<UWorld> LevelRef)
{
    if (LevelRef.IsNull()) return 0.0f;
    UWorld* World = GetWorld();
    if (!World) return 0.0f;

    // Stage 1: Disk -> RAM (80%) using Global Engine tracking
    FString PackageName = LevelRef.ToSoftObjectPath().GetLongPackageName();
    float LoadPercent = GetAsyncLoadPercentage(*PackageName);

    // If LoadPercent is -1, it's already in RAM (Stage 1 complete)
    float TotalProgress = (LoadPercent >= 0.0f) ? (LoadPercent / 100.0f) * 0.8f : 0.8f;

    // Stage 2: RAM -> World (20%)
    ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(World, FName(*LevelRef.GetAssetName()));
    if (Level)
    {
        if (Level->IsLevelVisible()) return 1.0f;
        if (Level->IsLevelLoaded()) TotalProgress = FMath::Max(TotalProgress, 0.9f);
    }
    return TotalProgress;
}

void UGJT_LevelManager::OnLevelShownCallback()
{
    bIsDoneLoading = true; // Signals the Latent Action to finish
    OnAfterLevelLoad.Broadcast(PreviousLevel, LoadingLevel);
    PreviousLevel = LoadingLevel;
}