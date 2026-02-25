// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GJT_PauseInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGJT_PauseInterface : public UInterface
{
    GENERATED_BODY()
};

class GAMEJAMTEMPLATE_API IGJT_PauseInterface
{
    GENERATED_BODY()

public:
    // TODO maybe one day pass the controller who paused it...
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GJT | UI")
    void OnPauseStateChanged(bool IsPaused);
};