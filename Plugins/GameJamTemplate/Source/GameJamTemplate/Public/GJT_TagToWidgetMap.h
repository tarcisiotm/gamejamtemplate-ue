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
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    FGameplayTag WidgetTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> WidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    bool bOneInstance = true;
};