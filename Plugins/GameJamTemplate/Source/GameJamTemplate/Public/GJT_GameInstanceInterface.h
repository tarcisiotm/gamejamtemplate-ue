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
    void SetGameContext(EGJT_GameContext NewGameContext);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT | GameState")
    EGJT_GameContext GetGameContext() const;

    // on context changed event?
};
