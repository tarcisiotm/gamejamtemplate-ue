// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GJT_Types.h"
//#include "GameplayTagContainer.h"

#include "GJT_WidgetInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(F_GJT_OnVisibilityEvent, TScriptInterface<IGJT_WidgetInterface>, WidgetInterface, EWidgetVisibilityState, NewVisibility);

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

    UFUNCTION(BlueprintNativeEvent, Category = "GJT | UI")
    void OnCancelRequested();

    virtual F_GJT_OnVisibilityEvent& GetOnVisibilityEvent() = 0;
    
    //virtual bool CanClose() = 0;

};