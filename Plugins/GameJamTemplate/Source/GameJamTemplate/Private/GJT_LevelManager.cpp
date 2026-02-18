#include "GJT_LevelManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "GJT_TransitionBase.h"
#include "GJT_DeveloperSettings.h"
#include "Blueprint/UserWidget.h"

class FGJT_LevelTransitionAction : public FPendingLatentAction
{
public:
    UGJT_LevelManager* Manager;
    const UObject* WorldContext;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

    FGJT_LevelTransitionAction(UGJT_LevelManager* InManager, const UObject* InContext, const FLatentActionInfo& LatentInfo)
        : Manager(InManager), WorldContext(InContext), ExecutionFunction(LatentInfo.ExecutionFunction)
        , OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {
    }

    virtual void UpdateOperation(FLatentResponse& Response) override
    {
        auto ProceedToLoading = [this]() {
            Manager->CurrentStage = ETransitionStage::Loading;
            Manager->InternalLoad(WorldContext);
        };

        auto HideTransition = [this]() {
            Manager->CurrentStage = ETransitionStage::HidingTransition;
            Manager->HideTransitionWidget();
        };

        if (!Manager) { Response.DoneIf(true); return; }

        GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("Transition Stage: %d | Fading: %d | Unloading: %d | Loading: %d"),
            (int32)Manager->CurrentStage, Manager->bWaitingForTransitionAnimation, Manager->bIsDoneUnloading, Manager->bIsDoneLoading));

        switch (Manager->CurrentStage)
        {
            case ETransitionStage::ShowingTransition: 
                if (Manager->bUsesTransition && Manager->bWaitingForTransitionAnimation) { break; }
            
                if (Manager->CurrentUnloadType != ESceneUnloadType::BeforeNewSceneLoads) 
                {
                    ProceedToLoading();
                    break;
                }

                Manager->CurrentStage = ETransitionStage::UnloadingBeforeNewScene;
                Manager->InternalUnload(WorldContext);

                break;
            case ETransitionStage::UnloadingBeforeNewScene:
                if (!Manager->bIsDoneUnloading) { break; }

                ProceedToLoading();
                break;
            case ETransitionStage::Loading:
                if (!Manager->bIsDoneLoading) { break; }

                if (Manager->CurrentUnloadType != ESceneUnloadType::AfterNewSceneLoads)
                {
                    HideTransition();
                    break;
                }

                Manager->CurrentStage = ETransitionStage::UnloadingAfterNewScene;
                Manager->InternalUnload(WorldContext);
                break;
            case ETransitionStage::UnloadingAfterNewScene:
                if (!Manager->bIsDoneUnloading) { break; }

                HideTransition();
                break;
            case ETransitionStage::HidingTransition:
                if (Manager->bUsesTransition && Manager->bWaitingForTransitionAnimation) { break; }
                Manager->CurrentStage = ETransitionStage::Finished;
    #if WITH_EDITOR
                //Manager->EditorBootstrapMapPath.Reset();
    #endif
                break;
        }
        
         Response.FinishAndTriggerIf(Manager->CurrentStage == ETransitionStage::Finished, ExecutionFunction, OutputLink, CallbackTarget);
    }
};

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

void UGJT_LevelManager::TransitionToLevel(const UObject* WorldContextObject, TSoftObjectPtr<UWorld> LevelRef, ESceneUnloadType UnloadType, FLatentActionInfo LatentInfo, bool usesTransition)
{
    bUsesTransition = usesTransition;
    UE_LOG(LogTemp, Warning, TEXT("LevelManager: Transition to level."));
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

    World->GetLatentActionManager().AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FGJT_LevelTransitionAction(this, WorldContextObject, LatentInfo));
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

void UGJT_LevelManager::OnLevelUnloadedCallback() { bIsDoneUnloading = true; }

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