// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"

#include "GJT_WidgetConfig.generated.h"

USTRUCT(BlueprintType)
struct FGJT_WidgetData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT")
    TSubclassOf<UUserWidget> WidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT", meta = (Bitmask, BitmaskEnum = "/Script/GameJamTemplate.EGJT_GameContext"))
    int32 AllowedContexts = 255;

};

UCLASS(BlueprintType)
class GAMEJAMTEMPLATE_API UGJT_WidgetConfig : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GJT")
    TMap<FGameplayTag, FGJT_WidgetData> WidgetMap;
};