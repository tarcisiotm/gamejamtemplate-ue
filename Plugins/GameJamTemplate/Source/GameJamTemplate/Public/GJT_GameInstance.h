// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GJT_GameInstance.generated.h"

UCLASS()
class GAMEJAMTEMPLATE_API UGJT_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "GJT | Scene Management")
    void TransitionToLevel(FName LevelName);

    UFUNCTION(BlueprintCallable, Category = "GJT | Scene Management")
    void ReportManagerReady(FName ManagerName);

protected:
    UPROPERTY(EditAnywhere, Category = "GJT | UI")
    TSubclassOf<class UUserWidget> TransitionWidgetClass;

    UPROPERTY()
    class UUserWidget* ActiveTransitionWidget;

    TSet<FName> PendingManagers;
    void CheckAllManagersReady();
};
