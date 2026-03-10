// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GJT_UIManagerInterface.h"
#include "GJT_PauseManagerInterface.h"
#include "GJT_TransitionInterface.h"
#include "GJT_WidgetConfig.h"

#include "GJT_UIManager.generated.h"

class FGameplayTags;
class IGJT_GameInstanceInterface;
class IGJT_WidgetInterface;

//struct F_GJT_UIStackEntry
//{
//	TObjectPtr<UWidget> Widget;
//	TSharedPtr<SWidget> FocusedWidget;
//};

/**
 * The UI Manager exists for the whole lifecycle of the game.
 * Show/Hide Widget will try to automatically play the default animations.
 * It will look at the widget config to know the context in which it can show the requested widget.
 */
UCLASS()
class GAMEJAMTEMPLATE_API UGJT_UIManager : public UGameInstanceSubsystem, public IGJT_UIManagerInterface
{
	GENERATED_BODY()
	
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	void ShowWidget_Implementation(FGameplayTag WidgetTag) override;
	void HideWidget_Implementation(FGameplayTag WidgetTag) override;
	void ProcessCancelRequest_Implementation() override;
	UFUNCTION(BlueprintCallable, Category = "GJT | UI")
	virtual FGameplayTag GetTopMostWidgetTag() const override final;

	virtual FOnTopMostWidgetChanged& GetOnTopMostWidgetChanged() override { return OnTopMostWidgetChanged; }

protected:
	FOnTopMostWidgetChanged OnTopMostWidgetChanged;
	//TArray<TWeakObjectPtr<UWidget>> FocusStack;
	//TArray<TSharedPtr<SWidget>> FocusStack;
	//TArray<F_GJT_UIStackEntry> FocusStack;

	void HandleWorldBeginPlay(UWorld* World, const UWorld::InitializationValues IValues);

	void InitializeTagToWidgetMap();

	TSubclassOf<UUserWidget> GetWidgetClassByTag(FGameplayTag WidgetTag);

	TObjectPtr<UUserWidget> GetOrCreateWidgetByTag(FGameplayTag WidgetTag);

	void BroadcastTopMostWidgetChanged();

	UFUNCTION()
	void OnPauseStateChanged(bool bInIsPaused);

	UFUNCTION()
	void OnVisibilityEvent(TScriptInterface<IGJT_WidgetInterface> WidgetInterface, EWidgetVisibilityState NewVisibility);

	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> WidgetStack;

	UPROPERTY()
	TObjectPtr<UUserWidget> PauseMenuWidget;

	UPROPERTY()
	TScriptInterface<IGJT_PauseManagerInterface> PauseWidgetInterface;

	UPROPERTY()
	TObjectPtr<UGJT_WidgetConfig> WidgetConfig;

	UPROPERTY()
	TMap<FGameplayTag, TSubclassOf<UUserWidget>> TagToWidgetMap;

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UUserWidget>> TagToSpawnedWidgetMap;

	UPROPERTY()
	TScriptInterface<IGJT_GameInstanceInterface> GameInstanceInterface;
};
