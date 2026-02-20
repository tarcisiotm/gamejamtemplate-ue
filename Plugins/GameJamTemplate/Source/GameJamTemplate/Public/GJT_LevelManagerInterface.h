#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GJT_Types.h"
#include "GJT_LevelManagerInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(F_GJT_OnLevelTransitionComplete, TSoftObjectPtr<UWorld>, OldLevel, TSoftObjectPtr<UWorld>, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_GJT_OnLevelTransitionProgress, float, Progress);

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
        bool bUsesTransition = true
    );

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GJT")
    float GetCurrentTransitionProgress();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GJT")
    FSoftObjectPath EditorOnly_GetEditorBootstrapMapPath() const;

    virtual void EditorOnly_SetEditorBootstrapMapPath(FSoftObjectPath EditorBootstrapMapPath) = 0;

    virtual F_GJT_OnLevelTransitionComplete& GetOnAfterLevelLoadedEvent() = 0;
    virtual F_GJT_OnFadeFinished& GetOnWidgetTransitionCompletedEvent() = 0;
    virtual F_GJT_OnLevelTransitionProgress& GetOnLevelTransitionProgressEvent() = 0;
};