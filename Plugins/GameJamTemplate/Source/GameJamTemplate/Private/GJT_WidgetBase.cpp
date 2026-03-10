// Copyright Tarcisio Games

#include "GJT_WidgetBase.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "MovieScene.h"
#include "Components/Widget.h"
#include "GameFramework/PlayerController.h"

void UGJT_WidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    // Setup interface wrapper
    CachedInterfaceWrapper.SetObject(this);
    CachedInterfaceWrapper.SetInterface(this);

    // Find animations
    ShowWidgetAnimationPtr = FindAnimation(FName("ShowWidgetAnimation"));
    HideWidgetAnimationPtr = FindAnimation(FName("HideWidgetAnimation"));

    // Bind animation finished delegates
    if (ShowWidgetAnimationPtr)
    {
        FWidgetAnimationDynamicEvent ShowDelegate;
        ShowDelegate.BindUFunction(this, FName("OnShowAnimationFinished"));
        BindToAnimationFinished(ShowWidgetAnimationPtr, ShowDelegate);
    }

    if (HideWidgetAnimationPtr)
    {
        FWidgetAnimationDynamicEvent HideDelegate;
        HideDelegate.BindUFunction(this, FName("OnHideAnimationFinished"));
        BindToAnimationFinished(HideWidgetAnimationPtr, HideDelegate);
    }

    if (!ShowWidgetAnimationPtr || !HideWidgetAnimationPtr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Optional Show/Hide animations missing!"));
    }
}

UWidgetAnimation* UGJT_WidgetBase::FindAnimation(FName AnimName) const
{
    if (UWidgetBlueprintGeneratedClass* WidgetClass = Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
    {
        for (UWidgetAnimation* Anim : WidgetClass->Animations)
        {
            if (Anim && Anim->GetMovieScene() && Anim->GetMovieScene()->GetFName() == AnimName)
            {
                return Anim;
            }
        }
    }
    return nullptr;
}

void UGJT_WidgetBase::Show_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Show_Implementation %s"), *GetName());
    SetVisibility(ESlateVisibility::Visible);
    BroadcastWidgetVisibilityEvent(ShowWidgetAnimationPtr ? EWidgetVisibilityState::FadingIn : EWidgetVisibilityState::Visible);

    if (ShowWidgetAnimationPtr)
    {
        PlayAnimation(ShowWidgetAnimationPtr);
    }
}

void UGJT_WidgetBase::Hide_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Hide_Implementation %s"), *GetName());

    BroadcastWidgetVisibilityEvent(HideWidgetAnimationPtr ? EWidgetVisibilityState::FadingOut : EWidgetVisibilityState::Hidden);

    if (HideWidgetAnimationPtr)
    {
        PlayAnimation(HideWidgetAnimationPtr);
    }
    else
    {
        SetVisibility(ESlateVisibility::Hidden);
    }
}

void UGJT_WidgetBase::OnCancelRequested_Implementation()
{
    //UE_LOG(LogTemp, Warning, TEXT("OnCancelRequested_Implementation %s"), *GetName());

    IGJT_WidgetInterface::Execute_Hide(this);
}

void UGJT_WidgetBase::SetFocusedWidget_Implementation(UWidget* NewFocusedWidget)
{
    if (!NewFocusedWidget || NewFocusedWidget == FocusedWidget) return;

    FocusedWidget = NewFocusedWidget;
    UE_LOG(LogTemp, Warning, TEXT("Focused Widget changed: %s"), *FocusedWidget->GetName());

    if (APlayerController* PC = GetOwningPlayer())
    {
        FocusedWidget->SetUserFocus(PC);
    }
}

void UGJT_WidgetBase::OnVisibilityChanged_Implementation(EWidgetVisibilityState NewVisibilityType)
{
    // Optional override in BP
}

void UGJT_WidgetBase::BroadcastWidgetVisibilityEvent(EWidgetVisibilityState NewVisibilityType)
{
    OnVisibilityChanged(NewVisibilityType);
    OnVisibilityEvent.Broadcast(CachedInterfaceWrapper, NewVisibilityType);
}

void UGJT_WidgetBase::OnShowAnimationFinished_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Show Animation Finished on %s"), *GetName());
}

void UGJT_WidgetBase::OnHideAnimationFinished_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Hide Animation Finished on %s"), *GetName());
    SetVisibility(ESlateVisibility::Collapsed);
    BroadcastWidgetVisibilityEvent(EWidgetVisibilityState::Hidden);
}