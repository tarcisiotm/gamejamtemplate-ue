// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GJT_AudioManagerInterface.h"

#include "GJT_AudioManager.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMTEMPLATE_API UGJT_AudioManager : public UGameInstanceSubsystem, public IGJT_AudioManagerInterface
{
	GENERATED_BODY()

public:
	UGJT_AudioManager();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	//UFUNCTION()
	void HandleWorldBeginPlay(UWorld* World, const UWorld::InitializationValues IValues);

protected:
	UPROPERTY()
	TMap<TSoftObjectPtr<UWorld>, UAudioComponent*> LevelToActiveBgmMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GJT | Audio")
	UAudioComponent* AudioComponent;

	UAudioComponent* CreateAudioComponent(float OwnerScene);

	UFUNCTION()
	void HandleOnBeforeLevelLoadedBroadcast(TSoftObjectPtr<UWorld> PreviousLevel, TSoftObjectPtr<UWorld> LoadedLevel);

	UFUNCTION()
	void HandleOnLevelLoadedBroadcast(TSoftObjectPtr<UWorld> PreviousLevel, TSoftObjectPtr<UWorld> LoadedLevel);
	
	//UFUNCTION()
	//void OnWorldReady(UWorld* World, const UWorld::InitializationValues IValues);

	void OnEngineInitComplete();

public:

	void PlayBGM_Implementation(USoundBase* Sound, float Volume, float Delay) override;

	void PlaySound2D_Implementation(const FVector Location, USoundBase* Sound, float Volume, float Delay) override;
	
};
