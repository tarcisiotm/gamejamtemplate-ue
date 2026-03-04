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
    void SetGameContext_Implementation(EGJT_GameContext NewGameContext) override { CurrentGameContext = NewGameContext; };

    //UFUNCTION(BlueprintCallable, Category = "GJT | State")
   // EGJT_GameContext GetGameState() const override final { return CurrentState; };
    virtual EGJT_GameContext GetGameContext_Implementation() const override;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "GJT | State")
    EGJT_GameContext CurrentGameContext = EGJT_GameContext::MainMenu;
};
