// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "GJT_PauseManagerInterface.h"

#include "GJT_PauseManager.generated.h"

UCLASS()
class GAMEJAMTEMPLATE_API UGJT_PauseManager : public UGameInstanceSubsystem, public IGJT_PauseManagerInterface
{
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	//UGJT_PauseManager();
	//~UGJT_PauseManager();

protected:
	bool bCanPause = false;
	bool bIsPaused = false;

	//UPROPERTY(BlueprintAssignable, Category = "GJT | Events")
	FOnPauseStateChanged OnPauseStateChangedEvent;

public:
	virtual FOnPauseStateChanged& GetOnPauseStateChangedEvent() override { return OnPauseStateChangedEvent; }

	bool IsPaused() const override;

	bool RequestPauseStateChange_Implementation(bool bInPause) override;

	bool TogglePauseState_Implementation() override;

	void SetCanPause_Implementation(bool bInCanPause) override;
};