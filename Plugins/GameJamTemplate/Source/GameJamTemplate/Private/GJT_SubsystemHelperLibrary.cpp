// Copyright Tarcisio Games


#include "GJT_SubsystemHelperLibrary.h"
#include "GJT_LevelManager.h" 

//UObject* UGJT_SubsystemHelperLibrary::GetLevelManager(const UObject* WorldContextObject)
//{
//    if (!WorldContextObject) return nullptr;
//    UWorld* World = WorldContextObject->GetWorld();
//    if (!World) return nullptr;
//
//    return World->GetGameInstance()->GetSubsystem<UGJT_LevelManager>();
//}

TScriptInterface<IGJT_LevelManagerInterface> UGJT_SubsystemHelperLibrary::GetLevelManagerInterface(const UObject* WorldContextObject)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World || !World->GetGameInstance()) return nullptr;

    UGJT_LevelManager* Subsystem = World->GetGameInstance()->GetSubsystem<UGJT_LevelManager>();

    TScriptInterface<IGJT_LevelManagerInterface> Interface;
    Interface.SetObject(Subsystem);
    Interface.SetInterface(Cast<IGJT_LevelManagerInterface>(Subsystem));

    return Interface;
}