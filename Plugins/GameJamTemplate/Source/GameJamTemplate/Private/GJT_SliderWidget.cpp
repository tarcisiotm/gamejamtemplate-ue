// Copyright Tarcisio Games


#include "GJT_SliderWidget.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Slider.h"

void UGJT_SliderWidget::NativePreConstruct()
{
    if (LabelText)
    {
        LabelText->SetText(LabelTextValue);
        LabelText->SetColorAndOpacity(FSlateColor(DefaultTextColor));
    }
}

void UGJT_SliderWidget::NativeConstruct()
{
    if (Slider)
    {
        Slider->OnValueChanged.AddDynamic(this, &UGJT_SliderWidget::HandleSliderValueChanged);
    }
}


void UGJT_SliderWidget::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
    if (LabelText)
    {
        LabelText->SetColorAndOpacity(FSlateColor(FocusedTextColor));
    }
}

void UGJT_SliderWidget::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
    if (LabelText)
    {
        LabelText->SetColorAndOpacity(FSlateColor(DefaultTextColor));
    }
}

void UGJT_SliderWidget::SetSliderValue(float Value)
{
    if (Value > Slider->MaxValue || Value < Slider->MinValue)
    {
        return;
    }

    Slider->SetValue(Value);
}

void UGJT_SliderWidget::HandleSliderValueChanged(float Value)
{
    float DisplayValue = Value * 100.f;
    ValueText->SetText(FText::AsNumber(FMath::RoundToInt(DisplayValue)));

    OnSliderValueChanged.Broadcast(Value);
}

