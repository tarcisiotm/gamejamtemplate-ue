// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GJT_Types.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "GJT_UIManagerInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTopMostWidgetChanged, UUserWidget*);

class APlayerController;

UINTERFACE(MinimalAPI)
class UGJT_UIManagerInterface : public UInterface
{
    GENERATED_BODY()
};

// on widget completely shown
// on widget completely hidden

class GAMEJAMTEMPLATE_API IGJT_UIManagerInterface
{
    GENERATED_BODY()

public:
    //virtual FOnPauseStateChanged& GetOnPauseStateChangedEvent() = 0;
    virtual FOnTopMostWidgetChanged& GetOnTopMostWidgetChanged() = 0;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT")
    void ShowWidget(FGameplayTag WidgetTag);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT")
    void HideWidget(FGameplayTag WidgetTag);

    // todo pass who requested the cancel...
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT")
    void ProcessCancelRequest();

    virtual FGameplayTag GetTopMostWidgetTag() const = 0;
};