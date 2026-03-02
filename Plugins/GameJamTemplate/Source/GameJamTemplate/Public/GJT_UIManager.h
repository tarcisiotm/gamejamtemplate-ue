// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GJT_UIManagerInterface.h"
#include "GJT_PauseManagerInterface.h"
#include "GJT_TransitionInterface.h"

#include "GJT_UIManager.generated.h"

class FGameplayTags;

/**
 * TODO write about the data setup and documentation here
 */
UCLASS()
class GAMEJAMTEMPLATE_API UGJT_UIManager : public UGameInstanceSubsystem, public IGJT_UIManagerInterface
{
	GENERATED_BODY()
	
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	void ShowWidget_Implementation(FGameplayTag WidgetTag) override;
	void HideWidget_Implementation(FGameplayTag WidgetTag) override;


protected:
	void HandleWorldBeginPlay(UWorld* World, const UWorld::InitializationValues IValues);

	void InitializeTagToWidgetMap();

	TSubclassOf<UUserWidget> GetWidgetClassByTag(FGameplayTag WidgetTag);

	TObjectPtr<UUserWidget> GetOrCreateWidgetByTag(FGameplayTag WidgetTag);

	UFUNCTION()
	void OnPauseStateChanged(bool bInIsPaused);

	UPROPERTY()
	TObjectPtr<UUserWidget> PauseMenuWidget;

	UPROPERTY()
	TScriptInterface<IGJT_PauseManagerInterface> PauseWidgetInterface;

	UDataTable* TagToWidgetDataTable;

	UPROPERTY()
	TMap<FGameplayTag, TSubclassOf<UUserWidget>> TagToWidgetMap;

	TMap<FGameplayTag, TObjectPtr<UUserWidget>> TagToSpawnedWidgetMap;

};
