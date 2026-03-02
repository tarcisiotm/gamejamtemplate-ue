// Copyright Tarcisio Games


#include "GJT_PlayerController.h"
#include "GJT_SubsystemHelperLibrary.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h" 
#include "InputActionValue.h"

void AGJT_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    //UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent"));

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        //UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent2"));

        if (PauseInputAction)
        {
            UE_LOG(LogTemp, Warning, TEXT("AGJT_PlayerController Binding pause"));
            EnhancedInputComponent->BindAction(PauseInputAction, ETriggerEvent::Triggered, this, &AGJT_PlayerController::HandlePauseRequest);
        }
    }
}

void AGJT_PlayerController::UpdateInputMode(bool bIsPaused)
{
    if (bIsPaused)
    {
        FInputModeGameAndUI Mode;
        Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        //Mode.SetHideCursorDuringCapture(false);

        SetInputMode(Mode);
        // TODO: if using mouse + keyboard
        bShowMouseCursor = true;
    }
    else
    {
        FInputModeGameOnly Mode;
        SetInputMode(Mode);
        // TODO: if using mouse + keyboard
        bShowMouseCursor = false;
    }
}

void AGJT_PlayerController::HandlePauseRequest(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("HandlePauseRequest"));
    if (!Value.Get<bool>())
    {
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("HandlePauseRequest2"));

    auto PauseInterface = UGJT_SubsystemHelperLibrary::GetPauseManagerInterface(this);

    if (PauseInterface.GetInterface())
    {
        UObject* PauseObj = PauseInterface.GetObject();

        UE_LOG(LogTemp, Warning, TEXT("Issued pause command?"));

        bool bDidIssueCommand = IGJT_PauseManagerInterface::Execute_TogglePauseState(PauseObj);

        if (bDidIssueCommand)
        {
            UE_LOG(LogTemp, Warning, TEXT("Issued pause command!"));

            bool bCurrentPauseState = PauseInterface->IsPaused();
            UpdateInputMode(bCurrentPauseState);
        }
    }
}