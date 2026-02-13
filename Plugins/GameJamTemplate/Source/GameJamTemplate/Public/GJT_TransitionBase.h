// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJT_TransitionBase.generated.h"

UENUM(BlueprintType)
enum class EFadeType : uint8
{
	FadeOut UMETA(DisplayName = "Fade Out (To Black)"),
	FadeIn  UMETA(DisplayName = "Fade In (To Game)")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFadeFinished, EFadeType, FadeType);

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

	UFUNCTION(BlueprintCallable, Category = "GJT")
	void BroadcastFadeOutFinished(EFadeType fadeType);

	UFUNCTION(BlueprintNativeEvent)
	void FadeIn();

	UFUNCTION(BlueprintNativeEvent)
	void FadeOut();
protected:
	UPROPERTY(EditAnywhere, Category = "GJT | Setup")
	float FadeInDuration = 0.25f;
	UPROPERTY(EditAnywhere, Category = "GJT | Setup")
	float FadeOutDuration = 0.25f;

	virtual void NativeConstruct() override;


	
};
