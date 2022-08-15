// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogCue.h"
#include "DialogCue_AnimationAndSound.generated.h"

/**
 * DialogCue with animations and an audio.
 */
UCLASS()
class MYGAMESYSTEMPLUGIN_API UDialogCue_AnimationAndSound : public UDialogCue
{
	GENERATED_BODY()

public:

	/**
	 * Start the audio of this cue.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogCue_AnimationAndSound|Audio")
	void StartAudio();

	/**
	 * Start the animation of this cue.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogCue_AnimationAndSound|Internal")
	void StartAnimation();

protected:

	virtual void OnCueBeginned_Implementation() override;
	virtual void OnCueEnded_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogCue_AnimationAndSound|Audio", meta = (BlueprintProtected))
	void PlayAudio();
	virtual void PlayAudio_Implementation();

	UFUNCTION(BlueprintCallable, Category = "DialogCue_AnimationAndSound|Audio", meta = (BlueprintProtected))
	void AudioPlayed();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogCue_AnimationAndSound|Animation", meta = (BlueprintProtected))
	void PlayAnimation();
	virtual void PlayAnimation_Implementation();

	UFUNCTION(BlueprintCallable, Category = "DialogCue_AnimationAndSound|Animation", meta = (BlueprintProtected))
	void AnimationPlayed(class UAnimMontage* AnimMontage, bool bInterrupted);

	UFUNCTION(BlueprintPure, Category = "DialogCue_AnimationAndSound|Interlocutors", meta = (BlueprintProtected))
	TArray<class UDialogueVoice*> GetInterlocutorsVoices(const TArray<class AActor*>& Interlocutors) const;

	UFUNCTION(BlueprintPure, Category = "DialogCue_AnimationAndSound|Interlocutors", meta = (BlueprintProtected))
	class AActor* GetOwnerOfVoice(const TArray<class AActor*>& Interlocutors, const class UDialogueVoice* Voice) const;

	UFUNCTION(BlueprintPure, Category = "DialogCue_AnimationAndSound|Animation", meta = (BlueprintProtected))
	bool HasAudioStartAnimNotify(const class UAnimMontage* Montage) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogCue_AnimationAndSound|Animation", meta = (BlueprintProtected))
	class UAnimMontage* Animation = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogCue_AnimationAndSound|Audio", meta = (BlueprintProtected))
	class UDialogueVoice* Voice = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogCue_AnimationAndSound|Audio", meta = (BlueprintProtected))
	class UDialogueWave* Sound = nullptr;

private:

	void CheckTransitionCondition();

	UAudioComponent* PlayingAudioComponent;
	UAnimInstance* PlayingAnimInstance;

	bool bIsAudioPlayed = false;
	bool bIsAnimationPlayed = false;
	
};
