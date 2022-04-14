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
	
	UFUNCTION(BlueprintCallable, Category = "MontagePlayerComponent")
	class UAudioComponent* PlayDialogWaveReplicated(class UDialogueWave* DialogWave, const TArray<class UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed = false, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 0.0f, bool bAutoDestroy = true);
	
	UFUNCTION(BlueprintCallable, Category = "MontagePlayerComponent")
	class UAudioComponent* PlayDialogWaveLocally(class UDialogueWave* DialogWave, const TArray<class UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed = false, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 0.0f, bool bAutoDestroy = true);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "MontagePlayerComponent")
	void PlayDialogWaveOnClients(class UDialogueWave* DialogWave, const TArray<class UDialogueVoice*>& Interlocutors, bool bStopWhenAttachedToDestroyed = false, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 0.0f, bool bAutoDestroy = true);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "MontagePlayerComponent")
	void StopDialogWaveReplicated();

	UFUNCTION(BlueprintCallable, Category = "MontagePlayerComponent")
	void StopDialogWaveLocally();

	UFUNCTION(BlueprintPure, Category = "MontagePlayerComponent")
	bool IsPlaying() const;

	UFUNCTION(BlueprintGetter, Category = "MontagePlayerComponent")
	class UDialogueVoice* GetSpeakerVoice() const { return SpeakerVoice; }
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetSpeakerVoice)
	class UDialogueVoice* SpeakerVoice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundAttenuation* Attenuation;

private:

	UPROPERTY()
	class UAudioComponent* PlayingAudioComponent = nullptr;
		
};
