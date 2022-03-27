// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogCue.h"
#include "DialogCue_AnimationAndSound.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogCueInfo_AnimationAndSound : public UDialogCueInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* Animation = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogueVoice* Voice = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogueWave* Sound = nullptr;

};

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UDialogCue_AnimationAndSound : public UDialogCue
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "DialogCue_AnimationAndSound|Audio")
	void StartAudio();

	UFUNCTION(BlueprintCallable, Category = "DialogCue_AnimationAndSound|Internal")
	void StartAnimation();

	virtual class UDialogCueInfo_AnimationAndSound* GetDialogUnitInfo_Implementation() override;

protected:

	virtual void OnCueBeginned_Implementation() override;
	virtual void OnCueEnded_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogCue_AnimationAndSound|Audio")
	void PlayAudio();
	virtual void PlayAudio_Implementation();

	UFUNCTION(BlueprintCallable, Category = "DialogCue_AnimationAndSound|Audio")
	void AudioPlayed();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogCue_AnimationAndSound|Animation")
	void PlayAnimation();
	virtual void PlayAnimation_Implementation();

	UFUNCTION(BlueprintCallable, Category = "DialogCue_AnimationAndSound|Animation")
	void AnimationPlayed(class UAnimMontage* AnimMontage, bool bInterrupted);

	UFUNCTION(BlueprintPure, Category = "DialogCue_AnimationAndSound|Interlocutors")
	TArray<class UDialogueVoice*> GetInterlocutorsVoices(TArray<class AActor*> Interlocutors);

	UFUNCTION(BlueprintPure, Category = "DialogCue_AnimationAndSound|Interlocutors")
	class AActor* GetOwnerOfVoice(TArray<class AActor*> Interlocutors, class UDialogueVoice* Voice);

	UFUNCTION(BlueprintPure, Category = "DialogCue_AnimationAndSound|Animation")
	bool HasAudioStartAnimNotify(class UAnimMontage* Montage);

private:

	void CheckTransitionCondition();

	UAudioComponent* PlayingAudioComponent;
	UAnimInstance* PlayingAnimInstance;

	bool bIsAudioPlayed = false;
	bool bIsAnimationPlayed = false;
	
};
