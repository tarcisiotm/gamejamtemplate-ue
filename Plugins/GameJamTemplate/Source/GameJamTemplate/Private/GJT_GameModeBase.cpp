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

    if (!World || this->EditorPersistentLevelRef.IsNull()) return;

    UGJT_LevelManager* LM = World->GetGameInstance()->GetSubsystem<UGJT_LevelManager>();
    if (!LM) return;

    FString RawPath = World->GetOutermost()->GetName();
    FSoftObjectPath CurrentLevelPath(UWorld::RemovePIEPrefix(RawPath));

    FString PersistentMapName = EditorPersistentLevelRef.GetAssetName();
    FString CurrentMapName = CurrentLevelPath.GetAssetName();

    // We are working in a level, load persistent and cache the current level
    if (!CurrentMapName.Equals(PersistentMapName, ESearchCase::IgnoreCase))
    {
        LM->EditorBootstrapMapPath = CurrentLevelPath;
        UGameplayStatics::OpenLevelBySoftObjectPtr(World, EditorPersistentLevelRef, true);
    }
    // We have arrived at the persistent level, reload the one we were working before
    else if (LM->EditorBootstrapMapPath.IsValid())
    {
        TSoftObjectPtr<UWorld> LevelRef{ LM->EditorBootstrapMapPath };
        LM->EditorBootstrapMapPath.Reset();

        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        //    FString::Printf(TEXT("BOOTSTRAP: Reloading %s"), *LevelRef.GetAssetName()));

        FLatentActionInfo LatentInfo(0, 999, TEXT("None"), this);
        LM->StreamLevelAsync(this, LevelRef, LatentInfo);
    }
#endif
}
