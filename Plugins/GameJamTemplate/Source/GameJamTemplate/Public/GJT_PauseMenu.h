// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJT_PauseInterface.h"
#include "GJT_Types.h"

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
	UPROPERTY(BlueprintAssignable, Category = "GJT | Events")
	F_GJT_OnFadeFinished OnFadeFinished;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GJT | Events")
	void Show();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GJT | Events")
	void Hide();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GJT | Events")
	void HandleTransitionDone(bool bIsPaused);


};
