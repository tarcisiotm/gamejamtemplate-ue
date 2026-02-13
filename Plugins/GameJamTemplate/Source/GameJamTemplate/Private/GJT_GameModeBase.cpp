// Copyright Tarcisio Games


#include "GJT_GameModeBase.h"

#if WITH_EDITOR
#include "GJT_LevelManager.h"
#include "Kismet/GameplayStatics.h"
#endif

void AGJT_GameModeBase::BeginPlay()
{
    Super::BeginPlay();

    EditorBootstrap();
}

// During a PIE session, this ensures the currently opened level
// in the editor is loaded via the persistent level.
void AGJT_GameModeBase::EditorBootstrap()
{
#if WITH_EDITOR
    UWorld* World = GetWorld();
    if (!World) return;

    UGJT_LevelManager* LM = World->GetGameInstance()->GetSubsystem<UGJT_LevelManager>();
    if (!LM || EditorPersistentLevelRef.IsNull()) return;

    FString CleanCurrentPath = UWorld::RemovePIEPrefix(World->GetOutermost()->GetName());

    FString PersistentPath = EditorPersistentLevelRef.GetLongPackageName();

    // First load: redirecting to persistent level
    if (!CleanCurrentPath.Equals(PersistentPath, ESearchCase::IgnoreCase))
    {
        LM->EditorBootstrapMapPath = FSoftObjectPath(*CleanCurrentPath);

        UGameplayStatics::OpenLevelBySoftObjectPtr(World, EditorPersistentLevelRef, true);
    }
    // Second load: arriving at the previously opened editor scene
    else if (LM->EditorBootstrapMapPath.IsValid())
    {
        // ARRIVAL: We match persistent, but have a "suitcase" to unpack
        TSoftObjectPtr<UWorld> LevelToLoad(LM->EditorBootstrapMapPath);

        LM->EditorBootstrapMapPath.Reset();

        FLatentActionInfo LatentInfo(0, FMath::Rand(), TEXT("None"), this);
        LM->StreamLevelAsync(this, LevelToLoad, LatentInfo);
    }
#endif
}
