// Copyright Tarcisio Games


#include "GJT_UIManager.h"
#include "GJT_SubsystemHelperLibrary.h"
#include "GJT_DeveloperSettings.h"
#include "GJT_GameplayTags.h"
#include "GJT_WidgetInterface.h"
#include "Slate/SObjectWidget.h"
void UGJT_UIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	InitializeTagToWidgetMap();

	TagToSpawnedWidgetMap.Empty();

	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UGJT_UIManager::HandleWorldBeginPlay);
}

void UGJT_UIManager::HandleWorldBeginPlay(UWorld* World, const UWorld::InitializationValues IValues)
{
	auto pauseManagerInterface = UGJT_SubsystemHelperLibrary::GetPauseManagerInterface(World);
	pauseManagerInterface->GetOnPauseStateChangedEvent().AddDynamic(this, &UGJT_UIManager::OnPauseStateChanged);

	GameInstanceInterface = UGJT_SubsystemHelperLibrary::GetGameInstanceInterface(this);
}

void UGJT_UIManager::InitializeTagToWidgetMap()
{
	const UGJT_DeveloperSettings* Settings = GetDefault<UGJT_DeveloperSettings>();

	if (Settings && !Settings->WidgetConfigAssetPath.IsNull())
	{
		WidgetConfig = Cast<UGJT_WidgetConfig>(Settings->WidgetConfigAssetPath.TryLoad());
	}

	if (!WidgetConfig) { UE_LOG(LogTemp, Error, TEXT("UI Manager: WidgetDataTable is NULL!")); }
}

void UGJT_UIManager::OnPauseStateChanged(bool bInIsPaused)
{
	if (bInIsPaused) { ShowWidget_Implementation(GJT_Tags::UI_Menu_Pause); }
	else { HideWidget_Implementation(GJT_Tags::UI_Menu_Pause); }
}

void UGJT_UIManager::OnVisibilityEvent(TScriptInterface<IGJT_WidgetInterface> WidgetInterface, EWidgetVisibilityState NewVisibility)
{
	if (NewVisibility != EWidgetVisibilityState::Hidden) { return; }

	if (WidgetInterface.GetObject())
	{
		UE_LOG(LogTemp, Warning, TEXT("-----Widget Interface belongs to Object: %s"), *WidgetInterface.GetObject()->GetName());
		UE_LOG(LogTemp, Warning, TEXT("-----Visibility event: %d"), (int32)NewVisibility);
	}

	UObject* WidgetObj = WidgetInterface.GetObject();
	UUserWidget* Widget = Cast<UUserWidget>(WidgetObj);

	if (Widget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Visibility event 2"));

		//bool bWasTopMost = (WidgetStack.Last() == Widget);

		WidgetStack.Remove(Widget);
		UE_LOG(LogTemp, Warning, TEXT("Stack size %d"), WidgetStack.Num());

		Widget->RemoveFromParent();

		//FocusStack.RemoveAll([Widget](const F_GJT_UIStackEntry& Entry)
		//{
		//	return Entry.Widget == Widget;
		//});

		//if (FocusStack.Num() > 0)
		//{
		//	F_GJT_UIStackEntry Previous = FocusStack.Pop();

		//	if (Previous.FocusedWidget.IsValid())
		//	{
		//		FSlateApplication::Get().SetUserFocus(0, Previous.FocusedWidget);
		//	}
		//}

		BroadcastTopMostWidgetChanged();

		// todo: if the stack is empty, return focus to the game
	}
}

TSubclassOf<UUserWidget> UGJT_UIManager::GetWidgetClassByTag(FGameplayTag WidgetTag)
{
	if (!WidgetConfig) { 
		UE_LOG(LogTemp, Warning, TEXT("No Widget Config. This is probably unintended!"));
		return nullptr; 
	}

	const FGJT_WidgetData* FoundData = WidgetConfig->WidgetMap.Find(WidgetTag);

	if (FoundData) { return FoundData->WidgetClass; }

	UE_LOG(LogTemp, Warning, TEXT("UI Manager: No class found for tag %s"), *WidgetTag.ToString());
	return nullptr;
}

TObjectPtr<UUserWidget> UGJT_UIManager::GetOrCreateWidgetByTag(FGameplayTag WidgetTag)
{
	// Have we already created this widget before?
	if (TagToSpawnedWidgetMap.Contains(WidgetTag))
	{
		TObjectPtr<UUserWidget>* FoundWidgetPtr = TagToSpawnedWidgetMap.Find(WidgetTag);

		if (FoundWidgetPtr) { return *FoundWidgetPtr; }
	}

	// Create it!
	TSubclassOf<UUserWidget> WidgetClass = GetWidgetClassByTag(WidgetTag);

	if (!WidgetClass) { return nullptr; } // error already handled by above Method

	TObjectPtr<UUserWidget> Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

	TagToSpawnedWidgetMap.Add(WidgetTag, Widget);

	return Widget;
}

void UGJT_UIManager::BroadcastTopMostWidgetChanged()
{
	UUserWidget* topMostWidget = WidgetStack.Num() > 0 ? WidgetStack.Last() : nullptr;
	//UUserWidget* topMostWidget = FocusStack.Num() > 0 ? FocusStack.Last().FocusedWidget : nullptr;
	OnTopMostWidgetChanged.Broadcast(topMostWidget);
}

void UGJT_UIManager::ShowWidget_Implementation(FGameplayTag WidgetTag)
{
	auto GameContext = IGJT_GameInstanceInterface::Execute_GetGameContext(GameInstanceInterface.GetObject());

	EGJT_GameContext CurrentState = static_cast<EGJT_GameContext>(static_cast<uint8>(GameContext));

	const FGJT_WidgetData* Data = WidgetConfig->WidgetMap.Find(WidgetTag);

	if (Data)
	{
		EGJT_GameContext AllowedMask = static_cast<EGJT_GameContext>(Data->AllowedContexts);

		bool bIsAllowed = EnumHasAnyFlags(AllowedMask, CurrentState); //UE_LOG(LogTemp, Warning, TEXT("UI Check | %s | Result: %s"), *WidgetTag.ToString(), bIsAllowed ? TEXT("ALLOWED") : TEXT("BLOCKED"));

		if (!bIsAllowed) { return; }
	}
	
	TObjectPtr<UUserWidget> Widget = GetOrCreateWidgetByTag(WidgetTag);

	if (!Widget || !Widget->GetClass()->ImplementsInterface(UGJT_WidgetInterface::StaticClass()))
	{
		return;
	}

	if (!Widget->IsInViewport())
	{
		Widget->AddToViewport(WidgetStack.Num());

		if (IGJT_WidgetInterface* WidgetInterface = Cast<IGJT_WidgetInterface>(Widget))
		{
			WidgetInterface->GetOnVisibilityEvent().AddDynamic(this, &UGJT_UIManager::OnVisibilityEvent);
		}

		UWidget* FinalFocusTarget = Widget;

		while (FinalFocusTarget && FinalFocusTarget->Implements<UGJT_WidgetFocusInterface>())
		{
			UWidget* NextTarget = IGJT_WidgetFocusInterface::Execute_GetDefaultFocusTarget(FinalFocusTarget);

			IGJT_WidgetFocusInterface::Execute_SetFocusedWidget(FinalFocusTarget, NextTarget);
			if (NextTarget == nullptr || NextTarget == FinalFocusTarget) { break; }

			FinalFocusTarget = NextTarget;
		}

		//F_GJT_UIStackEntry Entry;
		//Entry.Widget = FinalFocusTarget; // whatever is currently on top
		//Entry.FocusedWidget = FSlateApplication::Get().GetUserFocusedWidget(0);
		//FocusStack.Push(Entry);

		// only on pop?
		//if (FinalFocusTarget)
		//{
		//	TSharedPtr<SWidget> SlateWidget = FinalFocusTarget->GetCachedWidget();
		//	if (SlateWidget.IsValid())
		//	{
		//		FSlateApplication::Get().SetUserFocus(0, SlateWidget);
		//	}
		//}

		WidgetStack.AddUnique(Widget);

		UE_LOG(LogTemp, Warning, TEXT("Stack size %d"), WidgetStack.Num());

		IGJT_WidgetInterface::Execute_Show(Widget);

		BroadcastTopMostWidgetChanged();
	}

}

void UGJT_UIManager::HideWidget_Implementation(FGameplayTag WidgetTag)
{
	UE_LOG(LogTemp, Warning, TEXT("HideWidget_Implementation: %s"), *WidgetTag.ToString());
	TObjectPtr<UUserWidget> Widget = GetOrCreateWidgetByTag(WidgetTag);

	if (!Widget || !Widget->GetClass()->ImplementsInterface(UGJT_WidgetInterface::StaticClass()))
	{
		return;
	}

	IGJT_WidgetInterface::Execute_Hide(Widget);
}

void UGJT_UIManager::ProcessCancelRequest_Implementation()
{
	if (WidgetStack.Num() == 0) { return; }

	UUserWidget* TopWidget = WidgetStack.Last();

	if (TopWidget && TopWidget->GetClass()->ImplementsInterface(UGJT_WidgetInterface::StaticClass()))
	{
		IGJT_WidgetInterface::Execute_OnCancelRequested(TopWidget);
	}
}

FGameplayTag UGJT_UIManager::GetTopMostWidgetTag() const
{
	if (WidgetStack.Num() == 0) return FGameplayTag::EmptyTag;

	UUserWidget* TopWidget = WidgetStack.Last();

	for (const auto& Pair : TagToSpawnedWidgetMap)
	{
		if (Pair.Value == TopWidget)
		{
			return Pair.Key;
		}
	}

	return FGameplayTag::EmptyTag;
}