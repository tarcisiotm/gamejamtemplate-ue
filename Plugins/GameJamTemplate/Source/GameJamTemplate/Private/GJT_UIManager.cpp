// Copyright Tarcisio Games


#include "GJT_UIManager.h"
#include "GJT_SubsystemHelperLibrary.h"
#include "GJT_DeveloperSettings.h"
#include "GJT_GameplayTags.h"
#include "GJT_TagToWidgetMap.h"
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
}

void UGJT_UIManager::InitializeTagToWidgetMap()
{
	const UGJT_DeveloperSettings* Settings = GetDefault<UGJT_DeveloperSettings>();

	if (Settings && !Settings->TagToWidgetDataTablePath.IsNull())
	{
		TagToWidgetDataTable = Cast<UDataTable>(Settings->TagToWidgetDataTablePath.TryLoad());
	}

	if (!TagToWidgetDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("UI Manager: WidgetDataTable is NULL!"));
		return;
	}

	TagToWidgetMap.Empty();

	static const FString ContextString(TEXT("Widget Mapping Context"));
	TArray<FGJT_TagToWidgetMap*> Rows;
	TagToWidgetDataTable->GetAllRows<FGJT_TagToWidgetMap>(ContextString, Rows);

	for (FGJT_TagToWidgetMap* Row : Rows)
	{
		if (Row && Row->WidgetTag.IsValid() && Row->WidgetClass)
		{
			TagToWidgetMap.Add(Row->WidgetTag, Row->WidgetClass);
		}
	}
}

void UGJT_UIManager::OnPauseStateChanged(bool bInIsPaused)
{
	if (bInIsPaused) { ShowWidget_Implementation(GJT_Tags::UI_Menu_Pause); }
	else { HideWidget_Implementation(GJT_Tags::UI_Menu_Pause); }
}

TSubclassOf<UUserWidget> UGJT_UIManager::GetWidgetClassByTag(FGameplayTag WidgetTag)
{
	if (TSubclassOf<UUserWidget>* FoundClass = TagToWidgetMap.Find(WidgetTag))
	{
		return *FoundClass;
	}

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
	UE_LOG(LogTemp, Warning, TEXT("Show Widget"));
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