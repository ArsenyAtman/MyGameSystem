// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogCue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Dialog.h"
#include "DialogComponent.h"
#include "TalkableInterface.h"

void UDialogCue::Activate_Implementation()
{
	OnCueBeginned();
}

void UDialogCue::PlayNextDialogCue()
{
	EndCue();
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
	GetOwningDialog()->OnDialogUnitPassed(this, NextDialogUnit);
}
