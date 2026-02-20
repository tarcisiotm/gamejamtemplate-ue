// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJT_Types.h"
#include "GJT_TransitionInterface.h"
#include "GJT_TransitionBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMTEMPLATE_API UGJT_TransitionBase : public UUserWidget, public IGJT_TransitionInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "GJT | Events")
	F_GJT_OnFadeFinished OnFadeFinished;

	void Show_Implementation() override;

	void Hide_Implementation() override;

	virtual F_GJT_OnFadeFinished& GetOnFadeFinished() override { return OnFadeFinished; };

protected:
	bool bHasInit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Setup")
	float FadeInDuration = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Setup")
	float FadeOutDuration = 0.25f;

	// This is used so that the transition does not disappear too quickly in case the games loads too fast.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | Setup")
	float MinShownTime = 1.0f;

	float WidgetShownTimeStamp = 0;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT | Events")
	void InitializeWidget();

	UFUNCTION(BlueprintCallable, Category = "GJT")
	bool HasMinShownTimeElapsed();

	UFUNCTION(BlueprintCallable, Category = "GJT")
	void BroadcastFadeFinished(EFadeType fadeType);
};
