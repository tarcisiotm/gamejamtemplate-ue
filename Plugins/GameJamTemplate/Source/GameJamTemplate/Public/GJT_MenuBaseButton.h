// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJT_MenuBaseButton.generated.h"

class UButton;
class UTextBlock;
class USizeBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGJTButtonClicked);

/**
 *
 */
UCLASS()
class GAMEJAMTEMPLATE_API UGJT_MenuBaseButton : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "GJT | Events")
    FOnGJTButtonClicked OnButtonClicked;

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* Button;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text;

    UPROPERTY(meta = (BindWidget))
    USizeBox* SizeBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Appearance")
    FLinearColor DefaultTextColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Appearance")
    FLinearColor HoverTextColor = FLinearColor::Yellow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Appearance")
    FText ButtonText = FText::FromString("Button");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Appearance")
    float ButtonWidth= 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Appearance")
    float ButtonHeight = 40.f;

    UFUNCTION()
    void OnInternalButtonClicked();

    UFUNCTION(BlueprintNativeEvent)
    void OnClicked();

    UFUNCTION()
    void OnHovered();

    UFUNCTION()
    void OnUnhovered();

    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;

};