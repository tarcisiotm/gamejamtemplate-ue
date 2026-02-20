// Copyright Tarcisio Games


#include "GJT_GameModeBase.h"

#if WITH_EDITOR
#include "GJT_SubsystemHelperLibrary.h"
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

    auto levelManagerInterface = UGJT_SubsystemHelperLibrary::GetLevelManagerInterface(World);

    if (!levelManagerInterface || EditorPersistentLevelRef.IsNull()) return;

    FString CleanCurrentPath = UWorld::RemovePIEPrefix(World->GetOutermost()->GetName());

    // First load: redirecting to persistent level
    if (!CleanCurrentPath.Equals(PersistentPath, ESearchCase::IgnoreCase))
    {
        levelManagerInterface->EditorOnly_SetEditorBootstrapMapPath(FSoftObjectPath(*CleanCurrentPath));

        UGameplayStatics::OpenLevelBySoftObjectPtr(World, EditorPersistentLevelRef, true);
    }
    // Second load: arriving at the previously opened editor scene
    else if (levelManagerInterface->Execute_EditorOnly_GetEditorBootstrapMapPath(levelManagerInterface.GetObject()).IsValid())
    {
        TSoftObjectPtr<UWorld> LevelToLoad(levelManagerInterface->Execute_EditorOnly_GetEditorBootstrapMapPath(levelManagerInterface.GetObject()));

        IGJT_LevelManagerInterface::Execute_TransitionToLevel(
            levelManagerInterface.GetObject(),
            this,
            LevelToLoad,
            ESceneUnloadType::DoesNotUnload,
            false
        );
    }
#endif
}