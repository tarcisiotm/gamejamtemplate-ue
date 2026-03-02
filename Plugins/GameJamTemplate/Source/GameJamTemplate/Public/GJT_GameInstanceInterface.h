#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GJT_Types.h"
#include "GJT_GameInstanceInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGJT_GameInstanceInterface : public UInterface
{
    GENERATED_BODY()
};

class GAMEJAMTEMPLATE_API IGJT_GameInstanceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GJT | State")
    void SetGameState(EGJT_GameState NewState);

    virtual EGJT_GameState GetGameState() const = 0;

};
