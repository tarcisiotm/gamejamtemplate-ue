// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GJT_GameInstanceInterface.h"

#include "GJT_GameInstance.generated.h"

UCLASS()
class GAMEJAMTEMPLATE_API UGJT_GameInstance : public UGameInstance, public IGJT_GameInstanceInterface 
{
	GENERATED_BODY()
	
public:
    void SetGameState_Implementation(EGJT_GameState NewState) override { CurrentState = NewState; };

    UFUNCTION(BlueprintCallable, Category = "GJT | State")
    EGJT_GameState GetGameState() const override final { return CurrentState; };

protected:
    UPROPERTY(BlueprintReadOnly, Category = "GJT | State")
    EGJT_GameState CurrentState = EGJT_GameState::MainMenu;

  
};
