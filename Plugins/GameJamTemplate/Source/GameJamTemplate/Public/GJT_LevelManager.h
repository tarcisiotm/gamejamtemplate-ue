// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GJT_LevelManager.generated.h"

UCLASS() // <--- UHT needs this to wake up
class GAMEJAMTEMPLATE_API UGJT_LevelManager : public UGameInstanceSubsystem // <--- Added inheritance
{
    GENERATED_BODY() // <--- This connects to the .generated.h file

public:
    UFUNCTION(BlueprintCallable, Category = "GJT | Navigation")
    void LoadLevelByName(FName LevelName);

    UFUNCTION(BlueprintCallable, Category = "GJT | Navigation")
    void LoadLevelByReference(TSoftObjectPtr<UWorld> LevelRef);
};