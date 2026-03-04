// Copyright Tarcisio Games


#include "GJT_WidgetBase.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "MovieScene.h"

void UGJT_WidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    CachedInterfaceWrapper.SetObject(this);
    CachedInterfaceWrapper.SetInterface(Cast<IGJT_WidgetInterface>(this)); 
    
    ShowWidgetAnimationPtr = FindAnimation(FName("ShowWidgetAnimation"));
    HideWidgetAnimationPtr = FindAnimation(FName("HideWidgetAnimation"));

    if (ShowWidgetAnimationPtr)
    {
        FWidgetAnimationDynamicEvent ShowAnimDelegate;
        ShowAnimDelegate.BindDynamic(this, &UGJT_WidgetBase::OnShowAnimationFinished);

        BindToAnimationFinished(ShowWidgetAnimationPtr, ShowAnimDelegate);
    }

    if (HideWidgetAnimationPtr)
    {
        FWidgetAnimationDynamicEvent HideAnimDelegate;
        HideAnimDelegate.BindDynamic(this, &UGJT_WidgetBase::OnShowAnimationFinished);

        BindToAnimationFinished(HideWidgetAnimationPtr, HideAnimDelegate);
    }

    if (!ShowWidgetAnimationPtr || !HideWidgetAnimationPtr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Optional Show and/or Hide Animation missing. Please make sure this is intended!"));
    }
}

UWidgetAnimation* UGJT_WidgetBase::FindAnimation(FName AnimName) const
{
    UWidgetBlueprintGeneratedClass* WidgetClass = Cast<UWidgetBlueprintGeneratedClass>(GetClass());
    if (!WidgetClass) return nullptr;

    for (UWidgetAnimation* Anim : WidgetClass->Animations)
    {
        if (Anim && Anim->GetMovieScene())
        {
            if (Anim->GetMovieScene()->GetFName() == AnimName)
            {
                return Anim;
            }
        }
    }
    return nullptr;
}

void UGJT_WidgetBase::Show_Implementation()
{
    if (ShowWidgetAnimationPtr)
    {
        PlayAnimation(ShowWidgetAnimationPtr);
        SetVisibility(ESlateVisibility::Visible);
        //BroadcastWidgetVisibilityEvent(EWidgetVisibilityState::FadingIn);
    }else
    {
       SetVisibility(ESlateVisibility::Visible);
    }
}

void UGJT_WidgetBase::Hide_Implementation()
{
    if (HideWidgetAnimationPtr)
    {
        PlayAnimation(HideWidgetAnimationPtr);
        //BroadcastWidgetVisibilityEvent(EWidgetVisibilityState::FadingIn);
    }
    else
    {
        SetVisibility(ESlateVisibility::Hidden);
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
