// Copyright Tarcisio Games


#include "GJT_WidgetBase.h"

void UGJT_WidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    CachedInterfaceWrapper.SetObject(this);
    CachedInterfaceWrapper.SetInterface(Cast<IGJT_WidgetInterface>(this));

    if (ShowWidgetAnimation)
    {
        FWidgetAnimationDynamicEvent ShowAnimDelegate;
        ShowAnimDelegate.BindDynamic(this, &UGJT_WidgetBase::OnShowAnimationFinished);

        BindToAnimationFinished(ShowWidgetAnimation, ShowAnimDelegate);
    }

    if (HideWidgetAnimation)
    {
        FWidgetAnimationDynamicEvent HideAnimDelegate;
        HideAnimDelegate.BindDynamic(this, &UGJT_WidgetBase::OnShowAnimationFinished);

        BindToAnimationFinished(HideWidgetAnimation, HideAnimDelegate);
    }

    if (!ShowWidgetAnimation || !HideWidgetAnimation)
    {
        UE_LOG(LogTemp, Warning, TEXT("Show or Hide Animation missing. Please make sure this is intended!"));
    }
}

void UGJT_WidgetBase::Show_Implementation()
{
    //UE_LOG(LogTemp, Warning, TEXT("Show Implementation!"));
    if (ShowWidgetAnimation)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Play animation!"));
        PlayAnimation(ShowWidgetAnimation);
        SetVisibility(ESlateVisibility::Visible);

        //BroadcastWidgetVisibilityEvent(EWidgetVisibilityState::FadingIn);
    }else
    {
       //UE_LOG(LogTemp, Warning, TEXT("No Show for No show animation!"));
    }
}

void UGJT_WidgetBase::Hide_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("widget base Hide Implementation!"));

    if (HideWidgetAnimation)
    {
        UE_LOG(LogTemp, Warning, TEXT("widget base Hide animation!"));

        PlayAnimation(HideWidgetAnimation);

        //BroadcastWidgetVisibilityEvent(EWidgetVisibilityState::FadingIn);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Show for No show animation!"));
    }
}

void UGJT_WidgetBase::OnVisibilityChanged_Implementation(EWidgetVisibilityState NewVisibilityType)
{
}

void UGJT_WidgetBase::BroadcastWidgetVisibilityEvent(EWidgetVisibilityState NewVisibilityType)
{
    OnVisibilityChanged(NewVisibilityType);

    OnVisibilityEvent.Broadcast(CachedInterfaceWrapper, NewVisibilityType);
}

void UGJT_WidgetBase::OnShowAnimationFinished_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("OnShowAnimationFinished_Implementation"));
}

void UGJT_WidgetBase::OnHideAnimationFinished_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("OnHideAnimationFinished_Implementation"));

    SetVisibility(ESlateVisibility::Collapsed);
    BroadcastWidgetVisibilityEvent(EWidgetVisibilityState::Hidden);
}
