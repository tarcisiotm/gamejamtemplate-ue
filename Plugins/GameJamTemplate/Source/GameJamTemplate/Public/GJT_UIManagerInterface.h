// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GJT_Types.h"
#include "GameplayTagContainer.h"

#include "GJT_UIManagerInterface.generated.h"

class APlayerController;

UINTERFACE(MinimalAPI)
class UGJT_UIManagerInterface : public UInterface
{
    GENERATED_BODY()
};

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnPauseStateChanged, bool);
// on widget completely shown
// on widget completely hidden

class GAMEJAMTEMPLATE_API IGJT_UIManagerInterface
{
    GENERATED_BODY()

public:
    //virtual FOnPauseStateChanged& GetOnPauseStateChangedEvent() = 0;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT")
    void TogglePauseMenu(bool bInIsPaused, APlayerController* Instigator);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT")
    void ShowWidget(FGameplayTag WidgetTag);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT")
    void HideWidget(FGameplayTag WidgetTag);
};