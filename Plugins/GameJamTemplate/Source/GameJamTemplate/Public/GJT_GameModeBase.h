// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GJT_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMTEMPLATE_API AGJT_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "GJT | Bootstrap - Editor Only")
	TSoftObjectPtr<UWorld> EditorPersistentLevelRef;
#endif

	virtual void EditorBootstrap();
};
