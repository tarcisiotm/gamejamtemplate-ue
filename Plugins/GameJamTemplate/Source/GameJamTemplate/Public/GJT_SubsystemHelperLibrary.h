// Copyright Tarcisio Games


#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GJT_LevelManagerInterface.h"
#include "GJT_PauseManagerInterface.h"
#include "GJT_UIManagerInterface.h"

#include "GJT_SubsystemHelperLibrary.generated.h"

UCLASS()
class GAMEJAMTEMPLATE_API UGJT_SubsystemHelperLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, BlueprintCallable, Category = "GJT | Subsystems", meta = (WorldContext = "WorldContextObject"))
    static TScriptInterface<IGJT_LevelManagerInterface> GetLevelManagerInterface(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, BlueprintCallable, Category = "GJT | Subsystems", meta = (WorldContext = "WorldContextObject"))
    static TScriptInterface<IGJT_PauseManagerInterface> GetPauseManagerInterface(const UObject* WorldContextObject);
    
    UFUNCTION(BlueprintPure, BlueprintCallable, Category = "GJT | Subsystems", meta = (WorldContext = "WorldContextObject"))
    static TScriptInterface<IGJT_UIManagerInterface> GetUIManagerInterface(const UObject* WorldContextObject);

    template<typename InterfaceType, typename SubsystemType>
    static TScriptInterface<InterfaceType> GetSubsystemInterface(const UObject* WorldContextObject)
    {
        UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
        if (!World || !World->GetGameInstance()) return nullptr;

        SubsystemType* Subsystem = World->GetGameInstance()->GetSubsystem<SubsystemType>();
        if (!Subsystem) return nullptr;

        TScriptInterface<InterfaceType> Interface;
        Interface.SetObject(Subsystem);
        Interface.SetInterface(Cast<InterfaceType>(Subsystem));

        return Interface;
    }
};