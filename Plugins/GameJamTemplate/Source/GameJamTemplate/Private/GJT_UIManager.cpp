// Copyright Tarcisio Games


#include "GJT_UIManager.h"
#include "GJT_SubsystemHelperLibrary.h"
#include "GJT_DeveloperSettings.h"
#include "GJT_GameplayTags.h"
#include "GJT_WidgetInterface.h"

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

	UObject* WidgetObj = WidgetInterface.GetObject();
	UUserWidget* Widget = Cast<UUserWidget>(WidgetObj);

	if (Widget)
	{
		bool bWasTopMost = (WidgetStack.Last() == Widget);

		WidgetStack.Remove(Widget);
		Widget->RemoveFromParent();

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

		WidgetStack.AddUnique(Widget);
		IGJT_WidgetInterface::Execute_Show(Widget);

		BroadcastTopMostWidgetChanged();
	}

}

void UGJT_UIManager::HideWidget_Implementation(FGameplayTag WidgetTag)
{
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