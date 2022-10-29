// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogSystem/DialogUnit.h"

#include "DialogSystem/Dialog.h"

UDialog* UDialogUnit::GetOwningDialog() const
{
	return Cast<UDialog>(GetOuter());
}
