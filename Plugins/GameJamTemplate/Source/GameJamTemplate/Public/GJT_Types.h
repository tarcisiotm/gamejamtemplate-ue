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
    StartedFadingIn UMETA(DisplayName = "Fading In"),
    Visible UMETA(DisplayName = "Visible"),
    StartedFadingOut  UMETA(DisplayName = "Fading Out"),
    Hidden UMETA(DisplayName = "Hidden"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_GJT_OnFadeFinished, EFadeType, FadeType);

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
    None,
    PauseMenu UMETA(DisplayName = "Pause Menu"),
    TransitionScreen UMETA(DisplayName = "Transition Screen")
};