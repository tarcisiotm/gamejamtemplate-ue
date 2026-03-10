// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJT_PauseInterface.h"
#include "GJT_Types.h"
#include "GJT_WidgetBase.h"

#include "GJT_PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMTEMPLATE_API UGJT_PauseMenu : public UGJT_WidgetBase, public IGJT_PauseInterface
{
	GENERATED_BODY()
	
public:
	virtual void OnPauseStateChanged_Implementation(bool IsPaused) override;

protected:

	virtual void Show_Implementation() override;

	virtual void Hide_Implementation() override;
};
