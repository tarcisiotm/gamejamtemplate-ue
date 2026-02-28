// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
//#include "GJT_Types.h"
//#include "NativeGameplayTags.h"

#include "GJT_WidgetInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UGJT_WidgetInterface : public UInterface { GENERATED_BODY() };

class IGJT_WidgetInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, Category = "GJT | UI")
    void Show();

    UFUNCTION(BlueprintNativeEvent, Category = "GJT | UI")
    void Hide();

    //virtual FOnWidgetAnimationFinished& GetOnOpenedFinishedDelegate() = 0;
    //virtual FOnWidgetAnimationFinished& GetOnClosedFinishedDelegate() = 0;
};