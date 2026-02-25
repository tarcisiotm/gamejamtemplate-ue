// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJT_PauseInterface.h"

#include "GJT_PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMTEMPLATE_API UGJT_PauseMenu : public UUserWidget, public IGJT_PauseInterface
{
	GENERATED_BODY()
	
public:
	virtual void OnPauseStateChanged_Implementation(bool IsPaused) override;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GJT | Events")
	void HandleOnPaused();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GJT | Events")
	void HandleOnUnpaused();
};
