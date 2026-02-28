// Copyright Tarcisio Games


#include "GJT_SubsystemHelperLibrary.h"
#include "GJT_LevelManager.h" 
#include "GJT_PauseManager.h" 
#include "GJT_UIManager.h" 

TScriptInterface<IGJT_LevelManagerInterface> UGJT_SubsystemHelperLibrary::GetLevelManagerInterface(const UObject* WorldContextObject)
{
    return GetSubsystemInterface<IGJT_LevelManagerInterface, UGJT_LevelManager>(WorldContextObject);
}

TScriptInterface<IGJT_PauseManagerInterface> UGJT_SubsystemHelperLibrary::GetPauseManagerInterface(const UObject* WorldContextObject)
{
    return GetSubsystemInterface<IGJT_PauseManagerInterface, UGJT_PauseManager>(WorldContextObject);
}

TScriptInterface<IGJT_UIManagerInterface> UGJT_SubsystemHelperLibrary::GetUIManagerInterface(const UObject* WorldContextObject)
{
    return GetSubsystemInterface<IGJT_UIManagerInterface, UGJT_UIManager>(WorldContextObject);
}