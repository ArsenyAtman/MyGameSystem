// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogCue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Dialog.h"
#include "DialogComponent.h"
#include "TalkableInterface.h"

void UDialogCue::Activate_Implementation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		OnCueBeginned();
	}
}

void UDialogCue::PlayNextDialogCue()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		EndCue();
	}
}

void UDialogCue::EndCue()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		OnCueEnded();
		GetOwningDialog()->OnDialogUnitPassed(this, NextDialogUnit);
	}
}
