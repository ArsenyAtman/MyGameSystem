// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DialogCue.h"
#include "Components/SkeletalMeshComponent.h"
#include "TalkableInterface.h"
#include "DialogComponent.h"
#include "DialogCue_AnimationAndSound.h"
#include "Dialog.h"

UDialogCue_AnimationAndSound* UAnimNotify_DialogCue::GetCurrentDialogCue(const USkeletalMeshComponent* MeshComponent) const
{
	AActor* Owner = MeshComponent->GetOwner();
	if (IsValid(Owner) && Owner->GetLocalRole() == ENetRole::ROLE_Authority && Owner->Implements<UTalkableInterface>())
	{
		UDialogComponent* DialogComponent = ITalkableInterface::Execute_GetDialogComponent(Owner);
		if (IsValid(DialogComponent))
		{
			UDialog* CurrentDialog = DialogComponent->GetCurrentDialog();
			if (IsValid(CurrentDialog))
			{
				return Cast<UDialogCue_AnimationAndSound>(CurrentDialog->GetCurrentDialogUnit());
			}
		}
	}
	return nullptr;
}
