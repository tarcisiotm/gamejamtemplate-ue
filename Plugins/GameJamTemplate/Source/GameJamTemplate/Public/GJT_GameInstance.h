// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GJT_GameInstance.generated.h"

UENUM(BlueprintType)
enum class EGJT_GameState : uint8
{
    None,
    MainMenu,
    Loading,
    Gameplay,
    GameOver
};

UCLASS()
class GAMEJAMTEMPLATE_API UGJT_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    void SetGameState(EGJT_GameState NewState) { CurrentState = NewState; }

protected:
    UPROPERTY(BlueprintReadOnly, Category = "GJT | State")
    EGJT_GameState CurrentState = EGJT_GameState::MainMenu;

  
};
