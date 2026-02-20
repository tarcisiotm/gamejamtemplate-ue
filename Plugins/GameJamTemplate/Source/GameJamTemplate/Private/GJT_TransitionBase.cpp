// Copyright Tarcisio Games


#include "GJT_TransitionBase.h"
#include "GJT_LevelManager.h"

void UGJT_TransitionBase::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeWidget();

	UWorld* WorldPtr = GetWorld();
	if (!WorldPtr) { return; }

	UGJT_LevelManager* levelManager = WorldPtr->GetGameInstance()->GetSubsystem<UGJT_LevelManager>();
	if (!levelManager)
	{
		UE_LOG(LogTemp, Error, TEXT("Level Manager Subsystem not Found!!!"));
		return; 
	}
}

void UGJT_TransitionBase::InitializeWidget_Implementation()
{
	if (bHasInit) { return; }

	bHasInit = true;
}

void UGJT_TransitionBase::Show_Implementation()
{
}

void UGJT_TransitionBase::Hide_Implementation()
{
}

bool UGJT_TransitionBase::HasMinShownTimeElapsed()
{
	return (GetWorld()->GetTimeSeconds() - WidgetShownTimeStamp) >= MinShownTime;
}

void UGJT_TransitionBase::BroadcastFadeFinished(EFadeType fadeType)
{
	if (fadeType == EFadeType::FadeIn) 
	{
		WidgetShownTimeStamp = GetWorld()->GetTimeSeconds();
	}

	OnFadeFinished.Broadcast(fadeType);
}