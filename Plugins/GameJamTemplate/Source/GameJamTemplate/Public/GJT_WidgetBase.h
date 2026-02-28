// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJT_WidgetInterface.h"

#include "GJT_WidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMTEMPLATE_API UGJT_WidgetBase : public UUserWidget, public IGJT_WidgetInterface
{
	GENERATED_BODY()

public:
    // Interface Implementation
    virtual void Show_Implementation() override;
    virtual void Hide_Implementation() override;

    //virtual FOnWidgetAnimationFinished& GetOnOpenedFinishedDelegate() override { return OnOpenedFinished; }
    //virtual FOnWidgetAnimationFinished& GetOnClosedFinishedDelegate() override { return OnClosedFinished; }

protected:
    //UPROPERTY(BlueprintAssignable, Category = "GJT | UI")
    //FOnWidgetAnimationFinished OnOpenedFinished;

    //UPROPERTY(BlueprintAssignable, Category = "GJT | UI")
    //FOnWidgetAnimationFinished OnClosedFinished;

    // Helper for Blueprints to call when their UMG animations finish
    //UFUNCTION(BlueprintCallable, Category = "GJT | UI")
    //void BroadcastOpenedFinished() { OnOpenedFinished.Broadcast(); }
	
};
