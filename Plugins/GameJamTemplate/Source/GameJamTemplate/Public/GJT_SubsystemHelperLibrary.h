// Copyright Tarcisio Games


#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GJT_LevelManagerInterface.h"

#include "GJT_SubsystemHelperLibrary.generated.h"

UCLASS()
class UGJT_SubsystemHelperLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // This meta tag makes it look like a "native" node in BP
    //UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GJT | Navigation", meta = (WorldContext = "WorldContextObject"))
    //static UObject* GetLevelManager(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GJT | Navigation", meta = (WorldContext = "WorldContextObject"))
    static TScriptInterface<IGJT_LevelManagerInterface> GetLevelManagerInterface(const UObject* WorldContextObject);
};