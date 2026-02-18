#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GJT_Types.h"
#include "GJT_LevelManagerInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(F_GJT_OnLevelTransitionComplete, TSoftObjectPtr<UWorld>, OldLevel, TSoftObjectPtr<UWorld>, NewLevel);

UENUM(BlueprintType)
enum class ESceneUnloadType : uint8
{
    DoesNotUnload UMETA(DisplayName = "Does Not Unload"),
    BeforeNewSceneLoads UMETA(DisplayName = "Before New Scene Loads"),
    AfterNewSceneLoads  UMETA(DisplayName = "After New Scene Loads")
};

UINTERFACE(MinimalAPI)
class UGJT_LevelManagerInterface : public UInterface
{
    GENERATED_BODY()
};

class IGJT_LevelManagerInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GJT")
    void TransitionToLevel(
        const UObject* WorldContextObject,
        const TSoftObjectPtr<UWorld>& LevelRef,
        ESceneUnloadType UnloadType,
        //FLatentActionInfo LatentInfo,
        bool bUsesTransition = true
    );

    virtual F_GJT_OnLevelTransitionComplete& GetOnAfterLevelLoadedEvent() = 0;
    virtual F_GJT_OnFadeFinished& GetOnWidgetTransitionCompletedEvent() = 0;
};