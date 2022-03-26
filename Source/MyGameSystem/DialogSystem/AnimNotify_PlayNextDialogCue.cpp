// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayNextDialogCue.h"
#include "DialogCue_AnimationAndSound.h"

void UAnimNotify_PlayNextDialogCue::Notify(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation)
{
	UDialogCue_AnimationAndSound* CurrentDialogCue = GetCurrentDialogCue(MeshComponent);
	if (IsValid(CurrentDialogCue))
	{
		CurrentDialogCue->PlayNextDialogCue();
	}
}