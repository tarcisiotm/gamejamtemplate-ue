// Copyright Tarcisio Games


#include "GJT_AudioManager.h"
#include "GJT_LevelManagerInterface.h"
#include "GJT_SubsystemHelperLibrary.h"

UGJT_AudioManager::UGJT_AudioManager()
{
}

void UGJT_AudioManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UGJT_AudioManager::HandleWorldBeginPlay);
    auto World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("No world was found!"));
        return;
    }
}

void UGJT_AudioManager::HandleWorldBeginPlay(UWorld* World, const UWorld::InitializationValues IValues)
{
    auto levelManagerInterface = UGJT_SubsystemHelperLibrary::GetLevelManagerInterface(World);
    levelManagerInterface->GetOnAfterLevelLoadedEvent().AddDynamic(this, &UGJT_AudioManager::HandleOnLevelLoadedBroadcast);
}



void UGJT_AudioManager::HandleOnLevelLoadedBroadcast(TSoftObjectPtr<UWorld> PreviousLevel, TSoftObjectPtr<UWorld> LoadedLevel)
{

}

void UGJT_AudioManager::OnEngineInitComplete()
{
    UE_LOG(LogTemp, Warning, TEXT("OnEngineInitComplete"));

    auto World = GetWorld();
    if (!World) return;

    auto LM = UGJT_SubsystemHelperLibrary::GetLevelManagerInterface(World);

    // Check if the interface is valid using the bool operator or .GetInterface()
    if (LM)
    {
        LM->GetOnBeforeLevelLoadedEvent().AddUniqueDynamic(this, &UGJT_AudioManager::HandleOnBeforeLevelLoadedBroadcast);
        LM->GetOnAfterLevelLoadedEvent().AddUniqueDynamic(this, &UGJT_AudioManager::HandleOnLevelLoadedBroadcast);
    }
}

UAudioComponent* UGJT_AudioManager::CreateAudioComponent(float OwnerScene)
{
    UAudioComponent* NewAudioComp = NewObject<UAudioComponent>(this);

    if (NewAudioComp)
    {
        NewAudioComp->RegisterComponent();
        NewAudioComp->bIsUISound = true; // persists
        NewAudioComp->bAutoActivate = false;
    }

    return NewAudioComp;
}

// do we need both methods?
void UGJT_AudioManager::HandleOnBeforeLevelLoadedBroadcast(TSoftObjectPtr<UWorld> PreviousLevel, TSoftObjectPtr<UWorld> LoadedLevel)
{
    // fade out if an audio file is currently in the previous level
}

void UGJT_AudioManager::PlayBGM_Implementation(USoundBase* Sound, float Volume, float Delay)
{
	// detect if we have a BGM already playing for this scene, if we do?
	// if we do not, instantiate an audio component and set it
    // fade in or use whatever settings available to it
    // first version should only play
}

void UGJT_AudioManager::PlaySound2D_Implementation(const FVector Location, USoundBase* Sound, float Volume, float Delay)
{
}
