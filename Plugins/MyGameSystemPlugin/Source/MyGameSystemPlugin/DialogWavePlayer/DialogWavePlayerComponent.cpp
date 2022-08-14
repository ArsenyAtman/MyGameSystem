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

	// This component is replicated by default.
	SetIsReplicatedByDefault(true);
	
}

UAudioComponent* UDialogWavePlayerComponent::PlayDialogWaveReplicated(UDialogueWave* DialogWave, const TArray<UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed, float VolumeMultiplier, float PitchMultiplier, float StartTime, bool bAutoDestroy)
{
	// If has authority...
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		// than play the DialogWave on all clients.
		PlayDialogWaveOnClients(DialogWave, Interlocutors, bStopWhenAttachedToDestroyed, VolumeMultiplier, PitchMultiplier, StartTime, bAutoDestroy);
		// And return a new audio component on this machine.
		return PlayDialogWaveLocally(DialogWave, Interlocutors, bStopWhenAttachedToDestroyed, VolumeMultiplier, PitchMultiplier, StartTime, bAutoDestroy);
	}
	return nullptr;
}

UAudioComponent* UDialogWavePlayerComponent::PlayDialogWaveLocally(UDialogueWave* DialogWave, const TArray<UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed, float VolumeMultiplier, float PitchMultiplier, float StartTime, bool bAutoDestroy)
{
	// Stop the current playing dialog wave.
	StopDialogWaveLocally();

	// If the owner can play dialog waves...
	AActor* Actor = this->GetOwner();
	if (IsValid(Actor) && Actor->Implements<UDialogWavePlayableActorInterface>())
	{
		// than set up the dialog context, ...
		FDialogueContext Context;
		Context.Speaker = SpeakerVoice;
		Context.Targets = Interlocutors;
		// audio location and attachment, ...
		USceneComponent* AttachComponent = IDialogWavePlayableActorInterface::Execute_GetMouthComponent(Actor);
		FName AttachPoint = IDialogWavePlayableActorInterface::Execute_GetMouthPoint(Actor);
		FVector RelativeLocation = IDialogWavePlayableActorInterface::Execute_GetMouthRelativeLocation(Actor);
		// and finally play the dialog wave.
		PlayingAudioComponent = UGameplayStatics::SpawnDialogueAttached(DialogWave, Context, AttachComponent, AttachPoint, RelativeLocation, EAttachLocation::SnapToTarget, bStopWhenAttachedToDestroyed, VolumeMultiplier, PitchMultiplier, StartTime);
		return PlayingAudioComponent;
	}
	return nullptr;
}

void UDialogWavePlayerComponent::PlayDialogWaveOnClients_Implementation(UDialogueWave* DialogWave, const TArray<UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed, float VolumeMultiplier, float PitchMultiplier, float StartTime, bool bAutoDestroy)
{
	// If this machine is a client...
	if (GetOwnerRole() != ENetRole::ROLE_Authority)
	{
		// than play a new dialog wave on this machine.
		this->PlayDialogWaveLocally(DialogWave, Interlocutors, bStopWhenAttachedToDestroyed, VolumeMultiplier, PitchMultiplier, StartTime, bAutoDestroy);
	}
}

void UDialogWavePlayerComponent::StopDialogWaveReplicated_Implementation()
{
	// Stop the audio on this machine.
	StopDialogWaveLocally();
}

void UDialogWavePlayerComponent::StopDialogWaveLocally()
{
	// If the currently playing audio component is valid...
	if (IsValid(PlayingAudioComponent))
	{
		// and if it is playing...
		if(PlayingAudioComponent->IsPlaying())
		{
			// than stop it ...
			PlayingAudioComponent->Stop();
		}
		// and destroy it anyway.
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
