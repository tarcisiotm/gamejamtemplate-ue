// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "GJT_Types.generated.h" // <--- UHT needs this!

UENUM(BlueprintType)
enum class EFadeType : uint8
{
    None,
    FadeOut UMETA(DisplayName = "Fade Out (To Black)"),
    FadeIn  UMETA(DisplayName = "Fade In (To Game)")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_GJT_OnFadeFinished, EFadeType, FadeType);

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
    None,
    PauseMenu UMETA(DisplayName = "Pause Menu"),
    TransitionScreen UMETA(DisplayName = "Transition Screen")
};