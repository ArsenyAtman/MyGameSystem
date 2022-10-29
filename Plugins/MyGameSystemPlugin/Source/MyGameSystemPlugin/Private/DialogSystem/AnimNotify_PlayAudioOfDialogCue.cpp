// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogSystem/AnimNotify_PlayAudioOfDialogCue.h"

#include "DialogSystem/DialogCue_AnimationAndSound.h"

void UAnimNotify_PlayAudioOfDialogCue::Notify(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation)
{
	UDialogCue_AnimationAndSound* CurrentDialogCue = this->GetCurrentDialogCue(MeshComponent);
	if (IsValid(CurrentDialogCue))
	{
		CurrentDialogCue->StartAudio();
	}
}