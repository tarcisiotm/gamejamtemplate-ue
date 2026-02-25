// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "GJT_PauseManagerInterface.h"

class GAMEJAMTEMPLATE_API UGJT_PauseManager : public UGameInstanceSubsystem, public IGJT_PauseManagerInterface
{
public:
	UGJT_PauseManager();
	~UGJT_PauseManager();

protected:
	bool bCanPause = false;
	bool bIsPaused = false;

	UPROPERTY(BlueprintAssignable, Category = "GJT | Events")
	FOnPauseStateChanged OnPauseStateChangedEvent;

public:
	virtual FOnPauseStateChanged& GetOnPauseStateChangedEvent() override { return OnPauseStateChangedEvent; }

	void RequestPauseStateChange_Implementation(bool bInPause) override;

	void SetCanPause_Implementation(bool bInCanPause) override;
};