// Copyright Tarcisio Games

#include "GJT_LevelManager.h"
#include "Kismet/GameplayStatics.h"
#include "LatentActions.h"
#include "CoreMinimal.h"
#include "Engine/LevelStreamingDynamic.h"
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

    FName LevelName = FName(*LevelRef.GetAssetName());
    UGameplayStatics::OpenLevel(World, LevelName);
}

void UGJT_LevelManager::StreamLevelAsync(const UObject* WorldContextObject, TSoftObjectPtr<UWorld> LevelRef, FLatentActionInfo LatentInfo)
{
    if (LevelRef.IsNull()) return;

    bIsDoneLoading = false;

    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World) return;

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    Streamable.RequestAsyncLoad(
        LevelRef.ToSoftObjectPath(),
        FStreamableDelegate::CreateUObject(this, &UGJT_LevelManager::HandleLoadCompleted, WorldContextObject, LevelRef)
    );

    // Needed to make the complete node on the BP work without a (new) callback
    FLatentActionManager& LatentManager = World->GetLatentActionManager();
    LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
        new FGJT_LevelTransitionAction(&bIsDoneLoading, LatentInfo));
}

void UGJT_LevelManager::HandleLoadCompleted(const UObject* WorldContextObject, TSoftObjectPtr<UWorld> LevelRef)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World || !LevelRef.IsValid()) return;

    for (ULevelStreaming* Streaming : World->GetStreamingLevels())
    {
        if (!Streaming) continue;

        if (Streaming->GetWorldAssetPackageName().Contains(LevelRef.GetAssetName()))
        {
            Streaming->OnLevelShown.AddDynamic(this, &UGJT_LevelManager::OnLevelShownCallback);

            Streaming->SetShouldBeLoaded(true);
            Streaming->SetShouldBeVisible(true);
            World->FlushLevelStreaming(EFlushLevelStreamingType::Full); // spawn actors immediately
            break;
        }
    }

    PreviousLevel = nullptr;
    LoadingLevel = nullptr;
    bIsDoneLoading = true;
}

float UGJT_LevelManager::GetStreamingProgress(TSoftObjectPtr<UWorld> LevelRef)
{
    if (LevelRef.IsNull()) return -1.0f;

    ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(GetWorld(), *LevelRef.GetAssetName());
    if (Level)
    {
        if (Level->IsLevelLoaded()) return 1.0f;

        return Level->IsLevelVisible() ? 1.0f : 0.0f;
    }
    return 0.0f;
}


void UGJT_LevelManager::OnLevelShownCallback()
{
    UE_LOG(LogTemp, Warning, TEXT("On Level Shown Callback"));

    /*OnAfterLevelLoad.Broadcast(PreviousLevel, LoadingTargetLevel);

    if (!PreviousLevel.IsNull())
    {
        FLatentActionInfo UnloadInfo;
        UGameplayStatics::UnloadStreamLevel(this, FName(*PreviousLevel.GetAssetName()), UnloadInfo, false);
    }
*/
}
