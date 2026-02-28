// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInput/Public/InputActionValue.h"

#include "GJT_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMTEMPLATE_API AGJT_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Input")
    class UInputAction* PauseInputAction;

    UFUNCTION()
    void HandlePauseRequest(const FInputActionValue& Value);

    virtual void SetupInputComponent() override;

    void UpdateInputMode(bool bIsPaused);
	
};
