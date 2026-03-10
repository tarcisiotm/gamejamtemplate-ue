// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GJT_PauseManagerInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPauseStateChanged, bool, bIsPaused);

UINTERFACE(MinimalAPI)
class UGJT_PauseManagerInterface : public UInterface
{
    GENERATED_BODY()
};

class GAMEJAMTEMPLATE_API IGJT_PauseManagerInterface
{
    GENERATED_BODY()

public:
    virtual FOnPauseStateChanged& GetOnPauseStateChangedEvent() = 0;

    virtual bool IsPaused() const = 0;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT")
    bool RequestPauseStateChange(bool bInPause);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT")
    bool TogglePauseState();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT")
    void SetCanPause(bool bInCanPause);
};