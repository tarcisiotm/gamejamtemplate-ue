// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJT_WidgetFocusInterface.h"

#include "GJT_SliderWidget.generated.h"

//class UButton;
class UTextBlock;
class USizeBox;
class USlider;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGJTSliderValueChanged, float, Value);


/**
 * 
 */
UCLASS()
class GAMEJAMTEMPLATE_API UGJT_SliderWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override; 
	
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

	UFUNCTION(BlueprintCallable, Category = "GJT")
	void SetSliderValue(float Value);

	UPROPERTY(BlueprintAssignable, Category = "GJT | Events")
	FOnGJTSliderValueChanged OnSliderValueChanged;

	void HandleSliderValueChanged(float Value);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LabelText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ValueText;

	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox;

	UPROPERTY(meta = (BindWidget))
	USlider* Slider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Appearance")
	FLinearColor DefaultTextColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Appearance")
	FLinearColor FocusedTextColor = FLinearColor::Yellow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Appearance")
	FText LabelTextValue = FText::FromString("Button");
};
