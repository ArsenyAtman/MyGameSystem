// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogWavePlayerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UDialogWavePlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogWavePlayerComponent();
	
	// Without interlocutors (pick Context.Targets from DialogueWave.ContextMappings) ?

	UFUNCTION(BlueprintCallable, Category = "MontagePlayerComponent")
	class UAudioComponent* PlayDialogWaveReplicated(class UDialogueWave* DialogWave, const TArray<class UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed = false, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 0.0f, bool bAutoDestroy = true);
	
	// Without interlocutors (pick Context.Targets from DialogueWave.ContextMappings) ?

	UFUNCTION(BlueprintCallable, Category = "MontagePlayerComponent")
	class UAudioComponent* PlayDialogWaveLocally(class UDialogueWave* DialogWave, const TArray<class UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed = false, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 0.0f, bool bAutoDestroy = true);

	// Without interlocutors (pick Context.Targets from DialogueWave.ContextMappings) ?

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "MontagePlayerComponent")
	void PlayDialogWaveOnClients(class UDialogueWave* DialogWave, const TArray<class UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed = false, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 0.0f, bool bAutoDestroy = true);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "MontagePlayerComponent")
	void StopDialogWave();

	UFUNCTION(BlueprintPure)
	bool IsPlaying();
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogueVoice* SpeakerVoice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundAttenuation* Attenuation;

private:

	class UAudioComponent* PlayingAudioComponent = nullptr;
		
};
