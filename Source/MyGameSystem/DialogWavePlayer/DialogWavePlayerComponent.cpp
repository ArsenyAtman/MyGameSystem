// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogWavePlayerComponent.h"
#include "DialogWavePlayableActorInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/DialogueWave.h"
#include "Sound/DialogueVoice.h"
#include "Components/AudioComponent.h"

// Sets default values for this component's properties
UDialogWavePlayerComponent::UDialogWavePlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

UAudioComponent* UDialogWavePlayerComponent::PlayDialogWaveReplicated(UDialogueWave* DialogWave, const TArray<UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed, float VolumeMultiplier, float PitchMultiplier, float StartTime, bool bAutoDestroy)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		PlayDialogWaveOnClients(DialogWave, Interlocutors, bStopWhenAttachedToDestroyed, VolumeMultiplier, PitchMultiplier, StartTime, bAutoDestroy);
		PlayingAudioComponent = PlayDialogWaveLocally(DialogWave, Interlocutors, bStopWhenAttachedToDestroyed, VolumeMultiplier, PitchMultiplier, StartTime, bAutoDestroy);
		return PlayingAudioComponent;
	}
	return nullptr;
}

UAudioComponent* UDialogWavePlayerComponent::PlayDialogWaveLocally(UDialogueWave* DialogWave, const TArray<UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed, float VolumeMultiplier, float PitchMultiplier, float StartTime, bool bAutoDestroy)
{
	StopDialogWave();

	AActor* Actor = this->GetOwner();
	if (IsValid(Actor) && Actor->Implements<UDialogWavePlayableActorInterface>())
	{
		FDialogueContext Context;
		Context.Speaker = SpeakerVoice;
		Context.Targets = Interlocutors;
		USceneComponent* AttachComponent = IDialogWavePlayableActorInterface::Execute_GetMouthComponent(Actor);
		FName AttachPoint = IDialogWavePlayableActorInterface::Execute_GetMouthPoint(Actor);
		FVector RelativeLocation = IDialogWavePlayableActorInterface::Execute_GetMouthRelativeLocation(Actor);
		return UGameplayStatics::SpawnDialogueAttached(DialogWave, Context, AttachComponent, AttachPoint, RelativeLocation, EAttachLocation::KeepRelativeOffset, bStopWhenAttachedToDestroyed, VolumeMultiplier, PitchMultiplier, StartTime, Attenuation, bAutoDestroy);
	}
	return nullptr;
}

void UDialogWavePlayerComponent::PlayDialogWaveOnClients_Implementation(UDialogueWave* DialogWave, const TArray<UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed, float VolumeMultiplier, float PitchMultiplier, float StartTime, bool bAutoDestroy)
{
	if (GetOwnerRole() != ENetRole::ROLE_Authority)
	{
		PlayingAudioComponent = this->PlayDialogWaveLocally(DialogWave, Interlocutors, bStopWhenAttachedToDestroyed, VolumeMultiplier, PitchMultiplier, StartTime, bAutoDestroy);
	}
}

void UDialogWavePlayerComponent::StopDialogWave_Implementation()
{
	if (IsValid(PlayingAudioComponent))
	{
		if(PlayingAudioComponent->IsPlaying())
		{
			PlayingAudioComponent->Stop();
		}
		PlayingAudioComponent->DestroyComponent();
	}
	PlayingAudioComponent = nullptr;
}

bool UDialogWavePlayerComponent::IsPlaying() const
{
	if(IsValid(PlayingAudioComponent))
	{
		return PlayingAudioComponent->IsPlaying();
	}
	
	return false;
}