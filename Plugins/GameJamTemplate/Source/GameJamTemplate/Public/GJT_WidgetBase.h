// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJT_WidgetInterface.h"

#include "GJT_WidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMTEMPLATE_API UGJT_WidgetBase : public UUserWidget, public IGJT_WidgetInterface
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

public:
    // Interface Implementation
    virtual void Show_Implementation() override;
    virtual void Hide_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, Category = "GJT")
    void OnVisibilityChanged(EWidgetVisibilityState NewVisibilityType);

    void BroadcastWidgetVisibilityEvent(EWidgetVisibilityState NewVisibilityType);

    virtual F_GJT_OnVisibilityEvent& GetOnVisibilityEvent() override { return OnVisibilityEvent; };

protected:
    TScriptInterface<IGJT_WidgetInterface> CachedInterfaceWrapper;

    UWidgetAnimation* FindAnimation(FName AnimName) const;

    UPROPERTY(BlueprintReadOnly, Transient)
    class UWidgetAnimation* ShowWidgetAnimationPtr;
    //UPROPERTY(meta = (BindWidgetAnim, OptionalWidgetAnim), Transient)
    UPROPERTY(BlueprintReadOnly, Transient)

    class UWidgetAnimation* HideWidgetAnimationPtr;

    UPROPERTY(BlueprintAssignable, Category = "GJT | Events")
    F_GJT_OnVisibilityEvent OnVisibilityEvent;

    UFUNCTION(BlueprintNativeEvent, Category = "GJT | Events")
    void OnShowAnimationFinished();

    UFUNCTION(BlueprintNativeEvent, Category = "GJT | Events")
    void OnHideAnimationFinished();
};
