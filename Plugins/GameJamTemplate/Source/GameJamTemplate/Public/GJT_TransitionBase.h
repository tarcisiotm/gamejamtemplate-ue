// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJT_Types.h"
#include "GJT_TransitionBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMTEMPLATE_API UGJT_TransitionBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "GJT | Events")
	FOnFadeFinished OnFadeFinished;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT | Events")
	void Show();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT | Events")
	void Hide();
protected:
	bool bHasInit = false;

	UPROPERTY(EditAnywhere, Category = "GJT | Setup")
	float FadeInDuration = 0.25f;
	UPROPERTY(EditAnywhere, Category = "GJT | Setup")
	float FadeOutDuration = 0.25f;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT | Events")
	void InitializeWidget();

	/// <summary>
	/// Called from Blueprints
	/// </summary>
	/// <param name="fadeType"></param>
	UFUNCTION(BlueprintCallable, Category = "GJT")
	void BroadcastFadeFinished(EFadeType fadeType);
};
