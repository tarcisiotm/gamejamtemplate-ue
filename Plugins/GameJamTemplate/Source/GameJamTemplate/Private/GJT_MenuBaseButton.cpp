// Copyright Tarcisio Games

#include "GJT_MenuBaseButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void UGJT_MenuBaseButton::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (SizeBox)
    {
        SizeBox->SetWidthOverride(ButtonWidth);
        SizeBox->SetHeightOverride(ButtonHeight);
    }

    if (Text)
    {
        Text->SetText(ButtonText);
        Text->SetColorAndOpacity(FSlateColor(DefaultTextColor));
    }
}

void UGJT_MenuBaseButton::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &UGJT_MenuBaseButton::OnInternalButtonClicked);
        Button->OnHovered.AddDynamic(this, &UGJT_MenuBaseButton::OnHovered);
        Button->OnUnhovered.AddDynamic(this, &UGJT_MenuBaseButton::OnUnhovered);
    }
}

void UGJT_MenuBaseButton::OnClicked_Implementation()
{
}

void UGJT_MenuBaseButton::OnInternalButtonClicked()
{
    if (OnButtonClicked.IsBound())
    {
        OnButtonClicked.Broadcast();
    }
}

void UGJT_MenuBaseButton::OnHovered()
{
    if (Text)
    {
        Text->SetColorAndOpacity(FSlateColor(HoverTextColor));
    }
}

void UGJT_MenuBaseButton::OnUnhovered()
{
    if (Text)
    {
        Text->SetColorAndOpacity(FSlateColor(DefaultTextColor));
    }
}