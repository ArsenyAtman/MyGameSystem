// Fill out your copyright notice in the Description page of Project Settings.


#include "AxisActionInputHandler.h"
#include "Kismet/KismetSystemLibrary.h"

void UAxisActionInputHandler::Bind_Implementation(APlayerController* PlayerController, UObject* Object)
{
	Super::Bind_Implementation(PlayerController, Object);

	ReleaseActionDelegate.BindUFunction(Object, ReleaseFunctionName);
}

void UAxisActionInputHandler::HandleAxisInput_Implementation(float Value)
{
	if (GetCanHandleInput())
	{
		// If the value is greater than the "border" and the axis was released...
		if (Value > ActionAxisValue && bWasReleased)
		{
			// than handle this as a press action.
			Super::HandleAxisInput_Implementation(Value);
			bWasReleased = false;
		}
		// Else if the value is lower then the "border" and the axis was pressed...
		else if (Value < ActionAxisValue && !bWasReleased)
		{
			// than handle this as a release action.
			bWasReleased = true;
			HandleReleaseAction();
		}
	}
}

void UAxisActionInputHandler::HandleReleaseAction_Implementation()
{
	if(ReleaseActionDelegate.IsBound())
	{
		ReleaseActionDelegate.Execute();
	}

	InputHandled();
}

void UAxisActionInputHandler::Unbind_Implementation()
{
	Super::Unbind_Implementation();

	if (ReleaseActionDelegate.IsBound())
	{
		ReleaseActionDelegate.Unbind();
	}
}
