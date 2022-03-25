// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogUnitInterface.h"
#include "Delegates/IDelegateInstance.h"
#include "DialogSystemTypes.h"
#include "DialogCue.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogCue : public UObject, public IDialogUnitInterface
{
	GENERATED_BODY()

public:

	virtual void Activate_Implementation(class UDialog* OwnDialog) override;

	UFUNCTION(BlueprintCallable)
	void StartAudio();

	UFUNCTION(BlueprintCallable)
	void StartAnimation();

	UFUNCTION(BlueprintCallable)
	void PlayNextDialogCue();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CheckAvailabilityCondition(class UDialog* CheckingDialog);
	virtual bool CheckAvailabilityCondition_Implementation(class UDialog* CheckingDialog);

	UFUNCTION(BlueprintPure)
	struct FDialogCueStruct GetCueInfo(); //was FORCEINLINE

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UDialog* GetOwningDialog() { return OwningDialog; }

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnCueBeginned();
	void OnCueBeginned_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnCueEnded();
	void OnCueEnded_Implementation();

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

	UFUNCTION(BlueprintCallable)
	void EndCue();
	
	UFUNCTION(BlueprintCallable)
	bool HasAudioStartAnimNotify(class UAnimMontage* Montage);

	UPROPERTY(EditDefaultsOnly, meta = (MustImplement = "DialogUnitInterface"))
	TSubclassOf<UObject> NextDialogUnit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* Animation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogueVoice* Voice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogueWave* Sound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDialogCueType DialogCueType;

private:

	UAudioComponent* PlayingAudioComponent;
	UAnimInstance* PlayingAnimInstance;

	class UDialog* OwningDialog;

	bool bIsAudioPlayed = false;
	bool bIsAnimationPlayed = false;
	
};
