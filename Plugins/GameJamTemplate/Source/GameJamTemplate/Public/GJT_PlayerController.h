// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "InputMappingContext.h"

#include "GJT_PlayerController.generated.h"

class IGJT_PauseManagerInterface;
class IGJT_UIManagerInterface;
class UUserWidget;
class UWidget;

/**
 * 
 */
UCLASS()
class GAMEJAMTEMPLATE_API AGJT_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "GJT | Input")
    TObjectPtr<UInputMappingContext> IMC_UI;

    UPROPERTY(EditDefaultsOnly, Category = "GJT | Input")
    TObjectPtr<UInputMappingContext> IMC_Gameplay;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI Navigation")
    void HandleUINavigationBP(const FVector2D& InputVector);
    virtual void HandleUINavigationBP_Implementation(const FVector2D& InputVector);

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    UWidget* TopMostWidget;

    UPROPERTY()
    TScriptInterface<IGJT_PauseManagerInterface> PauseManagerInterface;

    UPROPERTY()
    TScriptInterface<IGJT_UIManagerInterface> UIManagerInterface;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Input")
    class UInputAction* UINavigationInputAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Input")
    class UInputAction* PauseInputAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Input")
    class UInputAction* CancelInputAction;

    UFUNCTION()
    void HandleUINavigation(const FInputActionValue& Value);

    UFUNCTION()
    void HandleInputPauseRequest(const FInputActionValue& Value);

    UFUNCTION()
    void TogglePauseRequest();

    UFUNCTION()
    void HandleCancelRequest(const FInputActionValue& Value);

    UFUNCTION()
    void HandleTopMostWidgetChanged(UUserWidget* NewTopMostWidget);

    virtual void SetupInputComponent() override;

    void UpdateInputMode(bool bIsUIActive, UWidget* FocusTargetWidget = nullptr);
};
