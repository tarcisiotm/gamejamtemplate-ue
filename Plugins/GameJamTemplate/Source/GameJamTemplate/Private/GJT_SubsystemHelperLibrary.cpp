// Copyright Tarcisio Games


#include "GJT_SubsystemHelperLibrary.h"
#include "GJT_LevelManager.h" 
#include "GJT_PauseManager.h" 

TScriptInterface<IGJT_LevelManagerInterface> UGJT_SubsystemHelperLibrary::GetLevelManagerInterface(const UObject* WorldContextObject)
{
    return GetSubsystemInterface<IGJT_LevelManagerInterface, UGJT_LevelManager>(WorldContextObject);
}

TScriptInterface<IGJT_PauseManagerInterface> UGJT_SubsystemHelperLibrary::GetPauseManagerInterface(const UObject* WorldContextObject)
{
    return GetSubsystemInterface<IGJT_PauseManagerInterface, UGJT_PauseManager>(WorldContextObject);
}