// Copyright Tarcisio Games


#include "GJT_WidgetBase.h"

void UGJT_WidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    CachedInterfaceWrapper.SetObject(this);
    CachedInterfaceWrapper.SetInterface(Cast<IGJT_WidgetInterface>(this));
}

void UGJT_WidgetBase::Show_Implementation(const FGameplayTag WidgetTag)
{
}

void UGJT_WidgetBase::Hide_Implementation(const FGameplayTag WidgetTag)
{
}

void UGJT_WidgetBase::OnVisibilityChanged_Implementation(EWidgetVisibilityState NewVisibilityType)
{
}

void UGJT_WidgetBase::BroadcastWidgetVisibilityEvent(EWidgetVisibilityState NewVisibilityType)
{
    OnVisibilityChanged(NewVisibilityType);

    OnVisibilityEvent.Broadcast(CachedInterfaceWrapper, NewVisibilityType);
}


