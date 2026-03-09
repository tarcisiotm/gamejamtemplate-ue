// Copyright Tarcisio Games


#include "GJT_PlayerController.h"
#include "GJT_SubsystemHelperLibrary.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h" 
#include "InputActionValue.h"
#include "GJT_GameplayTags.h"
#include "Blueprint/UserWidget.h"
#include "GJT_WidgetFocusInterface.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

void AGJT_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    PauseManagerInterface = UGJT_SubsystemHelperLibrary::GetPauseManagerInterface(this);
    UIManagerInterface = UGJT_SubsystemHelperLibrary::GetUIManagerInterface(this);

    UIManagerInterface->GetOnTopMostWidgetChanged().AddUObject(this, &AGJT_PlayerController::HandleTopMostWidgetChanged);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        if (UINavigationInputAction)
        {
            EnhancedInputComponent->BindAction(UINavigationInputAction, ETriggerEvent::Started, this, &AGJT_PlayerController::HandleUINavigation);
        }

        if (PauseInputAction)
        {
            EnhancedInputComponent->BindAction(PauseInputAction, ETriggerEvent::Triggered, this, &AGJT_PlayerController::HandleInputPauseRequest);
        }

        if (CancelInputAction)
        {
            EnhancedInputComponent->BindAction(CancelInputAction, ETriggerEvent::Triggered, this, &AGJT_PlayerController::HandleCancelRequest);
        }
    }
}

void AGJT_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(IMC_UI, 999);
    }
}

void AGJT_PlayerController::UpdateInputMode(bool bIsUIActive, UWidget* FocusTargetWidget)
{
    UWidget* targetWidget = FocusTargetWidget ? FocusTargetWidget : TopMostWidget;

    if (bIsUIActive)
    {
        FInputModeGameAndUI Mode;
        Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        Mode.SetHideCursorDuringCapture(false);

        if (targetWidget)
        {
            Mode.SetWidgetToFocus(targetWidget->TakeWidget());
            //targetWidget->SetFocus();
            targetWidget->SetUserFocus(this);
        }

        SetInputMode(Mode);
        FSlateApplication::Get().SetAllUserFocusToGameViewport();
        bShowMouseCursor = true;
    }
    else
    {
        FInputModeGameOnly Mode;
        SetInputMode(Mode);
        bShowMouseCursor = false;
        UE_LOG(LogTemp, Warning, TEXT("FInputModeGameOnly"));
    }
}

void AGJT_PlayerController::HandleUINavigation(const FInputActionValue& Value)
{
    FVector2D Axis = Value.Get<FVector2D>();

    EUINavigation Direction = EUINavigation::Invalid;
    if (Axis.Y > 0.5f)       Direction = EUINavigation::Up;
    else if (Axis.Y < -0.5f) Direction = EUINavigation::Down;
    else if (Axis.X < -0.5f) Direction = EUINavigation::Left;
    else if (Axis.X > 0.5f)  Direction = EUINavigation::Right;

    if (Direction != EUINavigation::Invalid)
    {
        TSharedPtr<SWidget> FocusedWidget = FSlateApplication::Get().GetUserFocusedWidget(0);
        if (FocusedWidget.IsValid())
        {
            FSlateApplication::Get().NavigateFromWidget(0, FocusedWidget, { Direction });
        }
    }

    HandleUINavigationBP(Axis);
}

void AGJT_PlayerController::HandleUINavigationBP_Implementation(const FVector2D& InputVector)
{
    UE_LOG(LogTemp, Warning, TEXT("HandleUINavigationBP called in C++ with X=%f Y=%f"), InputVector.X, InputVector.Y);
}

void AGJT_PlayerController::HandleInputPauseRequest(const FInputActionValue& Value)
{
    if (!Value.Get<bool>()) { return; }

    TogglePauseRequest();
}

void AGJT_PlayerController::TogglePauseRequest()
{
    //auto PauseInterface = UGJT_SubsystemHelperLibrary::GetPauseManagerInterface(this);

    if (!PauseManagerInterface.GetInterface())
    {
        UE_LOG(LogTemp, Error, TEXT("PauseManagerInterface unavailable!"));
        return;
    }

    if (PauseManagerInterface->IsPaused() && UIManagerInterface->GetTopMostWidgetTag() != GJT_Tags::UI_Menu_Pause)
    {
        UE_LOG(LogTemp, Warning, TEXT("Topmost widget is not Pause. Returning..."));
        return;
    }

    bool bDidIssueCommand = IGJT_PauseManagerInterface::Execute_TogglePauseState(PauseManagerInterface.GetObject());

    if (bDidIssueCommand)
    {
        bool bCurrentPauseState = PauseManagerInterface->IsPaused();
        UpdateInputMode(bCurrentPauseState);
    }
}

void AGJT_PlayerController::HandleCancelRequest(const FInputActionValue& Value)
{
    if (!Value.Get<bool>()) { return; }

    if (!UIManagerInterface.GetInterface())
    {
        UE_LOG(LogTemp, Error, TEXT("UIManagerInterface unavailable!"));
        return;
    }

    if (PauseManagerInterface->IsPaused() && UIManagerInterface->GetTopMostWidgetTag() == GJT_Tags::UI_Menu_Pause)
    {
        UE_LOG(LogTemp, Warning, TEXT("Topmost widget is Pause. Hijacking flow..."));
        TogglePauseRequest();
        return;
    }

    IGJT_UIManagerInterface::Execute_ProcessCancelRequest(UIManagerInterface.GetObject());
}

void AGJT_PlayerController::HandleTopMostWidgetChanged(UUserWidget* NewTopMostWidget)
{
    TopMostWidget = NewTopMostWidget;

    if (NewTopMostWidget)
    {
        // Start with the top-most widget
        UWidget* FinalFocusTarget = NewTopMostWidget;

        // Keep tunneling as long as the current target implements the interface
        while (FinalFocusTarget && FinalFocusTarget->Implements<UGJT_WidgetFocusInterface>())
        {
            UWidget* NextTarget = IGJT_WidgetFocusInterface::Execute_GetDefaultFocusTarget(FinalFocusTarget);

            IGJT_WidgetFocusInterface::Execute_SetFocusedWidget(FinalFocusTarget, NextTarget);
            // If the widget returns itself or nullptr, we've reached the end of the chain
            if (NextTarget == nullptr || NextTarget == FinalFocusTarget) { break; }

            FinalFocusTarget = NextTarget;
        }

        // Apply Input Mode and final focus
        UpdateInputMode(true, FinalFocusTarget);

        if (FinalFocusTarget)
        {
            FinalFocusTarget->SetFocus();
            TopMostWidget = FinalFocusTarget;
        }
    }
    else
    {
        UpdateInputMode(false);
    }
}