// Copyright Tarcisio Games

#include "GJT_LevelManager.h"
#include "Kismet/GameplayStatics.h"

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

    // Convert the reference to a string path that OpenLevel understands
    FName LevelName = FName(*LevelRef.GetAssetName());
    UGameplayStatics::OpenLevel(World, LevelName);
}