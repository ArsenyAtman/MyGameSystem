// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogCue.h"
#include "DialogCue_AnimationAndSound.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UDialogCue_AnimationAndSound : public UDialogCue
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void StartAudio();

	UFUNCTION(BlueprintCallable)
	void StartAnimation();

protected:

	virtual void OnCueBeginned_Implementation() override;
	virtual void OnCueEnded_Implementation() override;

	virtual FText GetSpeakerName_Implementation() override;
	virtual FText GetCueText_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayAudio();
	virtual void PlayAudio_Implementation();

	UFUNCTION(BlueprintCallable)
	void AudioPlayed();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayAnimation();
	virtual void PlayAnimation_Implementation();

	UFUNCTION(BlueprintCallable)
	void AnimationPlayed(class UAnimMontage* AnimMontage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	void CheckTransitionCondition();

	UFUNCTION(BlueprintPure)
	TArray<class UDialogueVoice*> GetInterlocutorsVoices(TArray<class AActor*> Interlocutors);

	UFUNCTION(BlueprintPure)
	class AActor* GetOwnerOfVoice(TArray<class AActor*> Interlocutors, class UDialogueVoice* Voice);

	UFUNCTION(BlueprintCallable)
	bool HasAudioStartAnimNotify(class UAnimMontage* Montage);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* Animation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogueVoice* Voice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogueWave* Sound;

private:

	UAudioComponent* PlayingAudioComponent;
	UAnimInstance* PlayingAnimInstance;

	bool bIsAudioPlayed = false;
	bool bIsAnimationPlayed = false;
	
};
