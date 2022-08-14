// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogWavePlayerComponent.generated.h"

/**
 * A component that plays DialogueWaves over the network.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEMPLUGIN_API UDialogWavePlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogWavePlayerComponent();
	
	/**
	 * Play a new dialog wave on all machines.
	 * @param DialogWave - New dialog wave to play.
	 * @param Interlocutors - Interlocutors of the current dialog except the talking interlocutor (the owner of this component).
	 * @param bStopWhenAttachedToDestroyed - Specifies whether the sound should stop playing when the owner its attached to is destroyed.
	 * @param VolumeMultiplier - A linear scalar multiplied with the volume, in order to make the sound louder or softer.
	 * @param PitchMultiplier - A linear scalar multiplied with the pitch.
	 * @param StartTime - How far in to the dialogue to begin playback at.
	 * @param bAutoDestroy - Whether the returned audio component will be automatically cleaned up when the sound finishes (by completing or stopping) or whether it can be reactivated.
	 * @return A new audio component that plays the dialog wave.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "MontagePlayerComponent")
	class UAudioComponent* PlayDialogWaveReplicated(class UDialogueWave* DialogWave, const TArray<class UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed = false, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 0.0f, bool bAutoDestroy = true);
	
	/**
	 * Play a new dialog wave on this machine.
	 * @param DialogWave - New dialog wave to play.
	 * @param Interlocutors - Interlocutors of the current dialog except the talking interlocutor (the owner of this component).
	 * @param bStopWhenAttachedToDestroyed - Specifies whether the sound should stop playing when the owner its attached to is destroyed.
	 * @param VolumeMultiplier - A linear scalar multiplied with the volume, in order to make the sound louder or softer.
	 * @param PitchMultiplier - A linear scalar multiplied with the pitch.
	 * @param StartTime - How far in to the dialogue to begin playback at.
	 * @param bAutoDestroy - Whether the returned audio component will be automatically cleaned up when the sound finishes (by completing or stopping) or whether it can be reactivated.
	 * @return A new audio component that plays the dialog wave.
	 */
	UFUNCTION(BlueprintCallable, Category = "MontagePlayerComponent")
	class UAudioComponent* PlayDialogWaveLocally(class UDialogueWave* DialogWave, const TArray<class UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed = false, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 0.0f, bool bAutoDestroy = true);

	/**
	 * Play a new dialog wave only on clients.
	 * @param DialogWave - New dialog wave to play.
	 * @param Interlocutors - Interlocutors of the current dialog except the talking interlocutor (the owner of this component).
	 * @param bStopWhenAttachedToDestroyed - Specifies whether the sound should stop playing when the owner its attached to is destroyed.
	 * @param VolumeMultiplier - A linear scalar multiplied with the volume, in order to make the sound louder or softer.
	 * @param PitchMultiplier - A linear scalar multiplied with the pitch.
	 * @param StartTime - How far in to the dialogue to begin playback at.
	 * @param bAutoDestroy - Whether the returned audio component will be automatically cleaned up when the sound finishes (by completing or stopping) or whether it can be reactivated.
	 */
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "MontagePlayerComponent")
	void PlayDialogWaveOnClients(class UDialogueWave* DialogWave, const TArray<class UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed = false, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 0.0f, bool bAutoDestroy = true);

	/**
	 * Stop the audio on all machines.
	 */
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "MontagePlayerComponent")
	void StopDialogWaveReplicated();

	/**
	 * Stop the audio on this machine.
	 */
	UFUNCTION(BlueprintCallable, Category = "MontagePlayerComponent")
	void StopDialogWaveLocally();

	/**
	 * Whether a dialog wave is currently playing.
	 * @return Is playing now.
	 */
	UFUNCTION(BlueprintPure, Category = "MontagePlayerComponent")
	bool IsPlaying() const;

	/**
	 * Return the voice.
	 * @return Voice of the speaker.
	 */
	UFUNCTION(BlueprintGetter, Category = "MontagePlayerComponent")
	class UDialogueVoice* GetSpeakerVoice() const { return SpeakerVoice; }
	
protected:

	/**
	 * Voice of the actor.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetSpeakerVoice, meta = (BlueprintProtected))
	class UDialogueVoice* SpeakerVoice;

	/**
	 * Attenuation of the speaker voice.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintProtected))
	class USoundAttenuation* Attenuation;

private:

	// Current playing audio.
	UPROPERTY()
	class UAudioComponent* PlayingAudioComponent = nullptr;
		
};
