// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DialogCue.h"
#include "Components/SkeletalMeshComponent.h"
#include "TalkableInterface.h"
#include "DialogComponent.h"
#include "DialogCue.h"
#include "Dialog.h"

UDialogCue* UAnimNotify_DialogCue::GetCurrentDialogCue(USkeletalMeshComponent* MeshComponent)
{
	AActor* Owner = MeshComponent->GetOwner();
	if (IsValid(Owner) && Owner->GetLocalRole() == ENetRole::ROLE_Authority && Owner->Implements<UTalkableInterface>())
	{
		UDialogComponent* DialogComponent = ITalkableInterface::Execute_GetDialogComponent(Owner);
		if (IsValid(DialogComponent))
		{
			UDialogComponent* MasterDialogComponent = DialogComponent->GetMasterDialogComponent();
			if (IsValid(MasterDialogComponent))
			{
				UDialog* CurrentDialog = MasterDialogComponent->GetCurrentDialog();
				if (IsValid(CurrentDialog))
				{
					return CurrentDialog->GetCurrentDialogCue();
				}
			}
		}
	}
	return nullptr;
}
