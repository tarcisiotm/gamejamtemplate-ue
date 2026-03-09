// Copyright Tarcisio Games


#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GJT_WidgetFocusInterface.generated.h"

USTRUCT(BlueprintType)
struct F_GJT_WidgetNavigation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UWidget* Up = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UWidget* Down = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UWidget* Left = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UWidget* Right = nullptr;
};

UINTERFACE(MinimalAPI)
class UGJT_WidgetFocusInterface : public UInterface
{
    GENERATED_BODY()
};

class GAMEJAMTEMPLATE_API IGJT_WidgetFocusInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT | UI")
    UWidget* GetDefaultFocusTarget() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GJT | UI")
    void SetFocusedWidget(UWidget* FocusedWidget);

    //UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    //void OnVerticalNavigate(int32 Direction);

    //UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    //void OnHorizontalNavigate(int32 Direction);
};