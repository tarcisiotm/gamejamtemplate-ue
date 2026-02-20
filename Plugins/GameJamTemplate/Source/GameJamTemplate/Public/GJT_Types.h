// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "GJT_Types.generated.h" // <--- UHT needs this!

UENUM(BlueprintType)
enum class EFadeType : uint8
{
    FadeOut UMETA(DisplayName = "Fade Out (To Black)"),
    FadeIn  UMETA(DisplayName = "Fade In (To Game)")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_GJT_OnFadeFinished, EFadeType, FadeType);