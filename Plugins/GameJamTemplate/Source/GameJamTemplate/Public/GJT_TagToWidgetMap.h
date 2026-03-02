// Copyright Tarcisio Games

#pragma once

#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "GJT_TagToWidgetMap.generated.h"

USTRUCT(BlueprintType)
struct FGJT_TagToWidgetMap : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GJT | UI")
    FGameplayTag WidgetTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GJT | UI")
    TSubclassOf<UUserWidget> WidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GJT | UI")
    bool bOneInstance = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GJT | UI", meta = (Bitmask, BitmaskEnum = "/Script/GameJamTemplate.EGJT_UIContext"))
    int32 AllowedContexts = static_cast<int32>(EGJT_UIContext::All);
};