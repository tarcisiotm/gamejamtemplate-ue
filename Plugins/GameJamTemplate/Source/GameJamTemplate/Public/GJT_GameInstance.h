// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GJT_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMTEMPLATE_API UGJT_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    // Call this from your buttons to start a travel
    UFUNCTION(BlueprintCallable, Category = "GJT | Scene Management")
    void TransitionToLevel(FName LevelName);

    // Call this from your "Managers" once they are initialized in the new level
    UFUNCTION(BlueprintCallable, Category = "GJT | Scene Management")
    void ReportManagerReady(FName ManagerName);

protected:
    UPROPERTY(EditAnywhere, Category = "GJT | UI")
    TSubclassOf<class UUserWidget> TransitionWidgetClass;

    UPROPERTY()
    class UUserWidget* ActiveTransitionWidget;

    // Track which managers we are waiting for
    TSet<FName> PendingManagers;

    void CheckAllManagersReady();
};
