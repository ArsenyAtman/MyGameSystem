// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogUnit.h"
#include "Dialog.h"

UDialog* UDialogUnit::GetOwningDialog() const
{
	return Cast<UDialog>(GetOuter());
}
