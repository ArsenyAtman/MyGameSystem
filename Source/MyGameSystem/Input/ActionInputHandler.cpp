// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionInputHandler.h"
#include "Kismet/KismetSystemLibrary.h"

void UActionInputHandler::Bind_Implementation(APlayerController* PlayerController, UObject* Object)
{
	if (IsValid(PlayerController) && IsValid(Object))
	{
		if (IsValid(PlayerController->InputComponent) && IsValid(Object->FindFunction(RelatedFunctionName)))
		{
			this->Unbind();
			InputActionBinding = PlayerController->InputComponent->BindAction(InputMappingName, KeyEvent, this, &UActionInputHandler::HandleActionInput);
			RelatedPlayerController = PlayerController;
			ActionDelegate.BindUFunction(Object, RelatedFunctionName);
		}
	}
}

void UActionInputHandler::HandleActionInput_Implementation()
{
	if (GetCanHandleInput() && ActionDelegate.IsBound())
	{
		ActionDelegate.Execute();
		InputHandled();
	}
}

void UActionInputHandler::Unbind_Implementation()
{
	if (ActionDelegate.IsBound())
	{
		ActionDelegate.Unbind();
	}

	if (IsValid(RelatedPlayerController))
	{
		if (IsValid(RelatedPlayerController->InputComponent))
		{
			RelatedPlayerController->InputComponent->RemoveActionBindingForHandle(InputActionBinding.GetHandle());
		}
	}
}