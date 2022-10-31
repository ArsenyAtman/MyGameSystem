// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogSystem/DialogCue.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DialogSystem/Dialog.h"
#include "DialogSystem/DialogComponent.h"
#include "DialogSystem/TalkableInterface.h"

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

bool UDialogCue::WasFired() const
{
	return GetOwningDialog()->WasCueFired(this->GetClass());
}

void UDialogCue::EndCue()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		OnCueEnded();
		GetOwningDialog()->OnDialogUnitPassed(this, NextDialogUnit);
	}
}
