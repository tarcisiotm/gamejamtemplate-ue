// Copyright Tarcisio Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/AudioComponent.h"

#include "GJT_AudioManagerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGJT_AudioManagerInterface : public UInterface
{
    GENERATED_BODY()
};

class GAMEJAMTEMPLATE_API IGJT_AudioManagerInterface
{
    GENERATED_BODY()

public:
    // TODO: get this to work first, and then figure out how to fade in / fade out automatically based on
    // knowing its owner scene
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GJT | Audio")
    void PlayBGM(USoundBase* Sound, float Volume, float Delay);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GJT | Audio")
    void PlaySound2D(const FVector Location, USoundBase* Sound, float Volume, float Delay);
};