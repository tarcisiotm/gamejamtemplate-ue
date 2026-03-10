// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"

#include "GJT_Types.generated.h"

UENUM(BlueprintType)
enum class EFadeType : uint8
{
    None,
    FadeOut UMETA(DisplayName = "Fade Out (To Black)"),
    FadeIn  UMETA(DisplayName = "Fade In (To Game)")
};

UENUM(BlueprintType)
enum class EWidgetVisibilityState : uint8
{
    None,
    FadingIn UMETA(DisplayName = "Fading In"),
    Visible UMETA(DisplayName = "Visible"),
    FadingOut  UMETA(DisplayName = "Fading Out"),
    Hidden UMETA(DisplayName = "Hidden"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_GJT_OnFadeFinished, EFadeType, FadeType);

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
    None,
    PauseMenu UMETA(DisplayName = "Pause Menu"),
    TransitionScreen UMETA(DisplayName = "Transition Screen"),
    SettingsMenu UMETA(DisplayName = "Settings Menu"),
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EGJT_GameContext : uint8
{
    SplashScreen = 0,
    LevelTransition = 1 << 0,
    MainMenu = 1 << 1,
    Gameplay = 1 << 2,
    Paused = 1 << 3,
    Credits = 1 << 4,
    All = 0xFF
};
ENUM_CLASS_FLAGS(EGJT_GameContext);