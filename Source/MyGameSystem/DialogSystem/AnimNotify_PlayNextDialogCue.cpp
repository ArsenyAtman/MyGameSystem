// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayNextDialogCue.h"
#include "DialogCue.h"

void UAnimNotify_PlayNextDialogCue::Notify(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation)
{
	UDialogCue* CurrentDialogCue = GetCurrentDialogCue(MeshComponent);
	if (IsValid(CurrentDialogCue))
	{
		CurrentDialogCue->PlayNextDialogCue();
	}
}