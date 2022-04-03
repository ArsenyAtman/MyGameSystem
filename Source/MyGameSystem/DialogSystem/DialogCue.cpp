// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogCue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Dialog.h"
#include "DialogComponent.h"
#include "TalkableInterface.h"
#include "DialogCueDataAsset.h"

void UDialogCue::Activate_Implementation(class UDialog* OwnDialog)
{
	OwningDialog = OwnDialog;
	OnCueBeginned();
}

void UDialogCue::PlayNextDialogCue()
{
	EndCue();
}

bool UDialogCue::CheckAvailabilityCondition_Implementation(UDialog* CheckingDialog)
{
	return true;
}

void UDialogCue::OnCueBeginned_Implementation()
{
	return;
}

void UDialogCue::OnCueEnded_Implementation()
{
	return;
}

void UDialogCue::EndCue()
{
	OnCueEnded();
	OwningDialog->OnDialogUnitPassed(this, CueData->NextDialogUnit);
}
