// Fill out your copyright notice in the Description page of Project Settings.


#include "AxisInputHandler.h"
#include "Kismet/KismetSystemLibrary.h"

void UAxisInputHandler::Bind_Implementation(APlayerController* PlayerController, UObject* Object)
{
	if (IsValid(PlayerController) && IsValid(Object))
	{
		if (IsValid(PlayerController->InputComponent) && IsValid(Object->FindFunction(RelatedFunctionName)))
		{
			this->Unbind();
			InputAxisBinding = PlayerController->InputComponent->BindAxis(InputMappingName, this, &UAxisInputHandler::HandleAxisInput);
			RelatedPlayerController = PlayerController;
			AxisDelegate.BindUFunction(Object, RelatedFunctionName);
		}
	}
}

void UAxisInputHandler::HandleAxisInput_Implementation(float Value)
{
	if (AxisDelegate.IsBound())
	{
		AxisDelegate.Execute(Value);
	}
}

void UAxisInputHandler::Unbind_Implementation()
{
	if (AxisDelegate.IsBound())
	{
		AxisDelegate.Unbind();
	}

	if (IsValid(RelatedPlayerController))
	{
		if (IsValid(RelatedPlayerController->InputComponent))
		{
			for (int i = 0; i < RelatedPlayerController->InputComponent->AxisBindings.Num(); )
			{
				FInputAxisBinding Binding = RelatedPlayerController->InputComponent->AxisBindings[i];
				if (Binding.AxisName == InputAxisBinding.AxisName && Binding.AxisDelegate.GetDelegateForManualSet().GetHandle() == InputAxisBinding.AxisDelegate.GetDelegateForManualSet().GetHandle())
				{
					RelatedPlayerController->InputComponent->AxisBindings.RemoveAt(i);
				}
				else
				{
					i++;
				}
			}
		}
	}
}