// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJT_WidgetInterface.h"
#include "GJT_WidgetFocusInterface.h"

#include "GJT_WidgetBase.generated.h"

USTRUCT(BlueprintType)
struct F_GJT_WidgetNavigationEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UWidget* Widget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    F_GJT_WidgetNavigation Navigation;
};

/**
 * 
 */
UCLASS()
class GAMEJAMTEMPLATE_API UGJT_WidgetBase : 
    public UUserWidget, public IGJT_WidgetInterface, public IGJT_WidgetFocusInterface
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

public:
    virtual void Show_Implementation() override;
    virtual void Hide_Implementation() override;
    virtual void OnCancelRequested_Implementation() override;

    virtual void SetFocusedWidget_Implementation(UWidget* NewFocusedWidget) override;

    UFUNCTION(BlueprintNativeEvent, Category = "GJT")
    void OnVisibilityChanged(EWidgetVisibilityState NewVisibilityType);

    void BroadcastWidgetVisibilityEvent(EWidgetVisibilityState NewVisibilityType);

    void OnVerticalNavigate_Implementation(int32 Direction);

    void OnHorizontalNavigate_Implementation(int32 Direction);

    virtual F_GJT_OnVisibilityEvent& GetOnVisibilityEvent() override { return OnVisibilityEvent; };

protected:
    TScriptInterface<IGJT_WidgetInterface> CachedInterfaceWrapper;

    UPROPERTY(BlueprintReadOnly)
    UWidget* FocusedWidget = nullptr;

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
