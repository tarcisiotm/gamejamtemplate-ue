// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GJT_TransitionBase.h"
#include "GJT_DeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Game Jam Template Settings"))
class GAMEJAMTEMPLATE_API UGJT_DeveloperSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    /** The Blueprint Widget used for level transitions */
    UPROPERTY(Config, EditAnywhere, Category = "UI")
    TSoftClassPtr<UGJT_TransitionBase> TransitionWidgetClass;
	
};
