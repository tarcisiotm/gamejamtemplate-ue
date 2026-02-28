// Copyright Tarcisio Games


#include "GJT_UIManager.h"
#include "GJT_SubsystemHelperLibrary.h"
#include "GJT_DeveloperSettings.h"
#include "GJT_GameplayTags.h"
#include "GJT_WidgetInterface.h"

void UGJT_UIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UGJT_UIManager::HandleWorldBeginPlay);
	auto World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("No world was found!"));
		return;
	}
}

void UGJT_UIManager::HandleWorldBeginPlay(UWorld* World, const UWorld::InitializationValues IValues)
{
	auto pauseManagerInterface = UGJT_SubsystemHelperLibrary::GetPauseManagerInterface(World);
	pauseManagerInterface->GetOnPauseStateChangedEvent().AddDynamic(this, &UGJT_UIManager::OnPauseStateChanged);
}

void UGJT_UIManager::OnPauseStateChanged(bool bInIsPaused)
{
	if (bInIsPaused)
	{
		UE_LOG(LogTemp, Warning, TEXT("Show Pause menu!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hide Pause menu!"));
	}
}

void UGJT_UIManager::TogglePauseMenu_Implementation(bool bInIsPaused, APlayerController* Instigator)
{
	if (bInIsPaused)
	{
		UE_LOG(LogTemp, Warning, TEXT("Show Pause menu!"));
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Hide Pause menu!"));
	}
}

void UGJT_UIManager::ShowWidget_Implementation(FGameplayTag WidgetTag)
{
	TSubclassOf<UUserWidget> WidgetClass;
	TObjectPtr<UUserWidget> Widget;
	TScriptInterface<IGJT_WidgetInterface> WidgetInterface;

	if (WidgetTag == GJT_Tags::UI_Menu_Pause)
	{
		WidgetClass = GetPauseMenuWidgetClass();
		//return;
	}

	if (WidgetTag == GJT_Tags::UI_Screen_Transition)
	{
		//WidgetClass = Get
		//return;
	}

	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to fetch Widget Class of Tag: %s"), *WidgetTag.ToString());
		return;
	}

	if (!Widget)
	{
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		WidgetInterface = Widget;

		if (!WidgetInterface)
		{
			UE_LOG(LogTemp, Error, TEXT("ActiveTransitionWidget does not implement IGJT_TransitionInterface!"));
		}

		//WidgetInterface->GetOnVisibilityEvent().AddDynamic(this, &UGJT_UIManager::HandleWidgetFadeFinished);
	}

	if (Widget && WidgetInterface) {
		if (!Widget->IsInViewport())
		{
			Widget->AddToViewport(9999);
		}

		//TransitionWidgetInterface->Execute_Show(TransitionWidgetInterface.GetObject());
	}
	
}

void UGJT_UIManager::HideWidget_Implementation(FGameplayTag WidgetTag)
{
}

// TODO: Move to Developer settings?
TSubclassOf<UUserWidget> UGJT_UIManager::GetPauseMenuWidgetClass() const
{
	const UGJT_DeveloperSettings* Settings = GetDefault<UGJT_DeveloperSettings>();
	return Settings ? Settings->PauseWidgetClass.LoadSynchronous() : nullptr;
}

//void UGJT_UIManager::ShowWidget()
//{
	/*
	if (!bUsesTransition) { return; }

	bWaitingForTransitionAnimation = true;
	TSubclassOf<UUserWidget> WidgetClass = GetTransitionWidgetClass();

	if (WidgetClass)
	{
		if (!ActiveTransitionWidget)
		{
			ActiveTransitionWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
			TransitionWidgetInterface = ActiveTransitionWidget;

			if (!TransitionWidgetInterface)
			{
				UE_LOG(LogTemp, Error, TEXT("ActiveTransitionWidget does not implement IGJT_TransitionInterface!"));
			}

			TransitionWidgetInterface->GetOnFadeFinished().AddDynamic(this, &UGJT_LevelManager::HandleWidgetFadeFinished);
		}

		if (ActiveTransitionWidget && TransitionWidgetInterface) {
			if (!ActiveTransitionWidget->IsInViewport())
			{
				ActiveTransitionWidget->AddToViewport(9999);
			}

			TransitionWidgetInterface->Execute_Show(TransitionWidgetInterface.GetObject());
		}
	}
	else bWaitingForTransitionAnimation = false;
	*/
//}