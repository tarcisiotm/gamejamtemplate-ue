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

	GameInstanceInterface = UGJT_SubsystemHelperLibrary::GetGameInstanceInterface(this);
}

void UGJT_UIManager::InitializeTagToWidgetMap()
{
	const UGJT_DeveloperSettings* Settings = GetDefault<UGJT_DeveloperSettings>();

	if (Settings && !Settings->WidgetConfigAssetPath.IsNull())
	{
		WidgetConfig = Cast<UGJT_WidgetConfig>(Settings->WidgetConfigAssetPath.TryLoad());
	}

	if (!WidgetConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("UI Manager: WidgetDataTable is NULL!"));
		return;
	}
}

void UGJT_UIManager::OnPauseStateChanged(bool bInIsPaused)
{
	if (bInIsPaused) { ShowWidget_Implementation(GJT_Tags::UI_Menu_Pause); }
	else { HideWidget_Implementation(GJT_Tags::UI_Menu_Pause); }
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

void UGJT_UIManager::ShowWidget_Implementation(FGameplayTag WidgetTag)
{
	auto GameContext = IGJT_GameInstanceInterface::Execute_GetGameContext(GameInstanceInterface.GetObject());

	EGJT_GameContext CurrentState =
		static_cast<EGJT_GameContext>(static_cast<uint8>(GameContext));

	const FGJT_WidgetData* Data = WidgetConfig->WidgetMap.Find(WidgetTag);

	if (Data)
	{
		EGJT_GameContext AllowedMask = static_cast<EGJT_GameContext>(Data->AllowedContexts);

		bool bIsAllowed = EnumHasAnyFlags(AllowedMask, CurrentState);
		//UE_LOG(LogTemp, Warning, TEXT("UI Check | %s | Result: %s"), *WidgetTag.ToString(), bIsAllowed ? TEXT("ALLOWED") : TEXT("BLOCKED"));

		if (!bIsAllowed) { return; }
	}

	TObjectPtr<UUserWidget> Widget = GetOrCreateWidgetByTag(WidgetTag);

	if (!Widget || !Widget->GetClass()->ImplementsInterface(UGJT_WidgetInterface::StaticClass()))
	{
		return;
	}

	if (!Widget->IsInViewport())
	{
		// todo properly pass the z order
		Widget->AddToViewport(9999);
	}

	IGJT_WidgetInterface::Execute_Show(Widget);
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