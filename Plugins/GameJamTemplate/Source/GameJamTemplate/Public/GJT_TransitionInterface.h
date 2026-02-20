// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GJT_Types.h"
#include "GJT_TransitionInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UGJT_TransitionInterface : public UInterface
{
    GENERATED_BODY()
};

class IGJT_TransitionInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT | Events")
    void Show();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT | Events")
    void Hide();

    virtual F_GJT_OnFadeFinished& GetOnF_GJT_OnFadeFinished() = 0;
};