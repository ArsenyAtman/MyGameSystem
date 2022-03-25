// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActorInterface.h"

// Add default functionality here for any IActionInterface functions that are not pure virtual.

FText IInteractableActorInterface::GetActionDescription_Implementation()
{
	return FText::FromString("None");
}

bool IInteractableActorInterface::CanInteract_Implementation(class AActor* Pawn)
{
	return true;
}