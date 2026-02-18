// Copyright Tarcisio Games


#include "GJT_GameModeBase.h"

#if WITH_EDITOR
#include "GJT_LevelManager.h"
#include "Kismet/GameplayStatics.h"
#endif

void AGJT_GameModeBase::BeginPlay()
{
    Super::BeginPlay();

#if WITH_EDITOR
        EditorBootstrap();
#endif
}

// During a PIE session, this ensures the currently opened level
// in the editor is loaded via the persistent level.
void AGJT_GameModeBase::EditorBootstrap()
{
#if WITH_EDITOR

    FString CurrentMapName = GetWorld()->GetMapName();
    FString PersistentPath = EditorPersistentLevelRef.GetLongPackageName();

    if (PersistentPath.IsEmpty() || CurrentMapName.Contains(PersistentPath))
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World) return;

    UGJT_LevelManager* LM = World->GetGameInstance()->GetSubsystem<UGJT_LevelManager>();
    if (!LM || EditorPersistentLevelRef.IsNull()) return;

    FString CleanCurrentPath = UWorld::RemovePIEPrefix(World->GetOutermost()->GetName());

    // First load: redirecting to persistent level
    if (!CleanCurrentPath.Equals(PersistentPath, ESearchCase::IgnoreCase))
    {
        LM->EditorBootstrapMapPath = FSoftObjectPath(*CleanCurrentPath);

        UGameplayStatics::OpenLevelBySoftObjectPtr(World, EditorPersistentLevelRef, true);
    }
    // Second load: arriving at the previously opened editor scene
    else if (LM->EditorBootstrapMapPath.IsValid())
    {
        TSoftObjectPtr<UWorld> LevelToLoad(LM->EditorBootstrapMapPath);


        FLatentActionInfo LatentInfo(0, FMath::Rand(), TEXT("None"), this);
        LM->TransitionToLevel(this, LevelToLoad, ESceneUnloadType::DoesNotUnload, LatentInfo, false);
    }
#endif
}
