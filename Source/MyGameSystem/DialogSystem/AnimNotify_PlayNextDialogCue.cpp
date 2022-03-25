// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayNextDialogCue.h"
#include "TalkableInterface.h"
#include "DialogComponent.h"
#include "DialogCue.h"
#include "Dialog.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotify_PlayNextDialogCue::Notify(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComponent->GetOwner();
	if (Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		if (Owner->Implements<UTalkableInterface>())
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
						UDialogCue* CurrentDialogCue = CurrentDialog->GetCurrentDialogCue();
						if (IsValid(CurrentDialogCue))
						{
							CurrentDialogCue->PlayNextDialogCue();
						}
					}
				}
			}
		}
	}
}