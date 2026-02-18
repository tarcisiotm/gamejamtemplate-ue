#include "GJT_LevelManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "GJT_TransitionBase.h"
#include "GJT_DeveloperSettings.h"
#include "Blueprint/UserWidget.h"

void UGJT_LevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    CurrentStage = ETransitionStage::Finished;
    bIsDoneLoading = true;
    bIsDoneUnloading = true;
#if WITH_EDITOR
    EditorBootstrapMapPath.Reset();
#endif
}

bool UGJT_LevelManager::IsTickable() const
{
    return CurrentStage != ETransitionStage::Finished;
}

void UGJT_LevelManager::Tick(float DeltaTime)
{
    UpdateCurrentLoadingStage(this); // todo remove this this
}

void UGJT_LevelManager::TransitionToLevel_Implementation(
    const UObject* WorldContextObject, 
    const TSoftObjectPtr<UWorld>& LevelRef, 
    ESceneUnloadType UnloadType, 
    bool usesTransition)
{
    bUsesTransition = usesTransition;

    if (CurrentStage != ETransitionStage::Finished)
    {
        UE_LOG(LogTemp, Warning, TEXT("LevelManager: Aborting! TransitionToLevel already in progress."));
        return; 
    }

    UWorld* World = GetWorld();
    if (!World || LevelRef.IsNull()) return;

    PendingLevel = LevelRef;
    CurrentUnloadType = UnloadType;
    bIsDoneLoading = false;
    bIsDoneUnloading = false;

    CurrentStage = ETransitionStage::ShowingTransition;
    ShowTransitionWidget();
}

void UGJT_LevelManager::UpdateCurrentLoadingStage(
    const UObject* WorldContextObject)
{
    auto ProceedToLoading = [this, WorldContextObject]() {
        CurrentStage = ETransitionStage::Loading;
        InternalLoad(WorldContextObject);
    };

    auto HideTransition = [this]() {
        CurrentStage = ETransitionStage::HidingTransition;
        HideTransitionWidget();
    };

    //GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("Transition Stage: %d | Fading: %d | Unloading: %d | Loading: %d"),
    //    (int32)CurrentStage, bWaitingForTransitionAnimation, bIsDoneUnloading, bIsDoneLoading));

    switch (CurrentStage)
    {
    case ETransitionStage::ShowingTransition:
        if (bUsesTransition && bWaitingForTransitionAnimation) { break; }

        if (CurrentUnloadType != ESceneUnloadType::BeforeNewSceneLoads)
        {
            ProceedToLoading();
            break;
        }

        CurrentStage = ETransitionStage::UnloadingBeforeNewScene;
        InternalUnload(WorldContextObject);

        break;
    case ETransitionStage::UnloadingBeforeNewScene:
        if (!bIsDoneUnloading) { break; }

        ProceedToLoading();
        break;
    case ETransitionStage::Loading:
        if (!bIsDoneLoading) { break; }

        if (CurrentUnloadType != ESceneUnloadType::AfterNewSceneLoads)
        {
            HideTransition();
            break;
        }

        CurrentStage = ETransitionStage::UnloadingAfterNewScene;
        InternalUnload(WorldContextObject);
        break;
    case ETransitionStage::UnloadingAfterNewScene:
        if (!bIsDoneUnloading) { break; }

        HideTransition();
        break;
    case ETransitionStage::HidingTransition:
        if (bUsesTransition && bWaitingForTransitionAnimation) { break; }
        CurrentStage = ETransitionStage::Finished;
#if WITH_EDITOR
        //EditorBootstrapMapPath.Reset();
#endif
        break;
    }
}

void UGJT_LevelManager::InternalLoad(const UObject* WorldContextObject)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World) return;

    bIsDoneLoading = false;
    LoadingLevel = PendingLevel;

    FLatentActionInfo LatentInfo;
    LatentInfo.UUID = FMath::Rand();
    UGameplayStatics::LoadStreamLevelBySoftObjectPtr(World, PendingLevel, true, false, LatentInfo);

    FString TargetAssetName = PendingLevel.GetAssetName();
    ULevelStreaming* FoundLevel = nullptr;

    for (ULevelStreaming* Streaming : World->GetStreamingLevels())
    {
        if (Streaming && Streaming->GetWorldAsset().GetAssetName().Equals(TargetAssetName, ESearchCase::IgnoreCase))
        {
            FoundLevel = Streaming;
            break;
        }
    }

    if (FoundLevel)
    {
        if (FoundLevel->IsLevelVisible())
        {
            OnLevelShownCallback();
        }
        else
        {
            FoundLevel->OnLevelShown.AddUniqueDynamic(this, &UGJT_LevelManager::OnLevelShownCallback);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("LevelManager: Could not find streaming level for %s"), *TargetAssetName);

        OnLevelShownCallback();
    }
}

void UGJT_LevelManager::InternalUnload(const UObject* WorldContextObject)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World) { bIsDoneUnloading = true; return; }

    TSoftObjectPtr<UWorld> Target = (CurrentUnloadType == ESceneUnloadType::AfterNewSceneLoads) ? PreviousLevel : GetCurrentLevelReference(WorldContextObject);

    // Safety check: Don't try to unload the persistent level or a null reference
    if (Target.IsNull() || Target.GetAssetName() == World->GetName())
    {
        bIsDoneUnloading = true;
        return;
    }

    bIsDoneUnloading = false;
    UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(World, Target, FLatentActionInfo(), false);

    FString TargetPackage = Target.ToSoftObjectPath().GetLongPackageName();
    bool bFound = false;
    for (ULevelStreaming* Streaming : World->GetStreamingLevels())
    {
        if (Streaming && Streaming->GetWorldAssetPackageName().Equals(TargetPackage, ESearchCase::IgnoreCase))
        {
            Streaming->OnLevelUnloaded.AddUniqueDynamic(this, &UGJT_LevelManager::OnLevelUnloadedCallback);
            bFound = true;
            break;
        }
    }

    if (!bFound) { bIsDoneUnloading = true; }
}

void UGJT_LevelManager::OnLevelShownCallback() 
{ 
    bIsDoneLoading = true; 
    OnAfterLevelLoad.Broadcast(PreviousLevel, LoadingLevel); 
    PreviousLevel = LoadingLevel; 
}

void UGJT_LevelManager::OnLevelUnloadedCallback() 
{ 
    bIsDoneUnloading = true; 
}

void UGJT_LevelManager::ShowTransitionWidget()
{
    if (!bUsesTransition) { return; }

    bWaitingForTransitionAnimation = true;
    TSubclassOf<UGJT_TransitionBase> WidgetClass = GetTransitionWidgetClass();

    if (WidgetClass)
    {
        if (!ActiveTransitionWidget) 
        {
            ActiveTransitionWidget = CreateWidget<UGJT_TransitionBase>(GetWorld(), WidgetClass); 
            ActiveTransitionWidget->OnFadeFinished.AddDynamic(this, &UGJT_LevelManager::HandleWidgetFadeFinished);
        }
        if (ActiveTransitionWidget) { 
            if (!ActiveTransitionWidget->IsInViewport()) 
            {
                ActiveTransitionWidget->AddToViewport(9999);
            }
            ActiveTransitionWidget->Show(); 
        }
    }
    else bWaitingForTransitionAnimation = false;
}

void UGJT_LevelManager::HideTransitionWidget()
{
    if (!bUsesTransition) { return; }

    bWaitingForTransitionAnimation = true;

    if (ActiveTransitionWidget) ActiveTransitionWidget->Hide();
    else bWaitingForTransitionAnimation = false;
}

// todo: refactor
float UGJT_LevelManager::GetGlobalProgress() const
{
    switch (CurrentStage) {
    case ETransitionStage::ShowingTransition: return 0.1f;
    case ETransitionStage::UnloadingBeforeNewScene: return 0.2f;
    case ETransitionStage::Loading: return 0.2f + (GetStreamingProgress(PendingLevel) * 0.7f);
    case ETransitionStage::HidingTransition: return 0.95f;
    case ETransitionStage::Finished: return 1.0f;
    default: return 0.0f;
    }
}

float UGJT_LevelManager::GetStreamingProgress(TSoftObjectPtr<UWorld> LevelRef) const
{
    if (LevelRef.IsNull()) return 0.0f;
    FString PackageName = LevelRef.ToSoftObjectPath().GetLongPackageName();
    float LoadPercent = GetAsyncLoadPercentage(*PackageName);
    float Total = (LoadPercent >= 0.0f) ? (LoadPercent / 100.0f) * 0.8f : 0.8f;
    if (UWorld* World = GetWorld()) {
        if (ULevelStreaming* L = UGameplayStatics::GetStreamingLevel(World, FName(*LevelRef.GetAssetName()))) {
            if (L->IsLevelVisible()) return 1.0f;
            if (L->IsLevelLoaded()) Total = FMath::Max(Total, 0.9f);
        }
    }
    return Total;
}

TSoftObjectPtr<UWorld> UGJT_LevelManager::GetCurrentLevelReference(const UObject* WorldContextObject)
{
    ULevelStreaming* L = GetCurrentLevelStreamingObject(WorldContextObject);
    return L ? TSoftObjectPtr<UWorld>(L->GetWorldAsset().ToSoftObjectPath()) : nullptr;
}

ULevelStreaming* UGJT_LevelManager::GetCurrentLevelStreamingObject(const UObject* WorldContextObject)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World) return nullptr;
    for (ULevelStreaming* L : World->GetStreamingLevels()) { if (L && L->IsLevelVisible()) return L; }
    return nullptr;
}

TSubclassOf<UGJT_TransitionBase> UGJT_LevelManager::GetTransitionWidgetClass() const
{
    const UGJT_DeveloperSettings* Settings = GetDefault<UGJT_DeveloperSettings>();
    return Settings ? Settings->TransitionWidgetClass.LoadSynchronous() : nullptr;
}

void UGJT_LevelManager::HandleWidgetFadeFinished(EFadeType FadeType)
{
    OnTransitionFinished.Broadcast(FadeType);
    bWaitingForTransitionAnimation = false;
}