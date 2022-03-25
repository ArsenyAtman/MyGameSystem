// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerWithInputHandlers.h"
#include "Kismet/GameplayStatics.h"
#include "InputHandler.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

APlayerControllerWithInputHandlers::APlayerControllerWithInputHandlers()
{
	//...
}

void APlayerControllerWithInputHandlers::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent)
	{
		for (UInputHandler* InputHandler : InputHandlers)
		{
			InputHandler->Bind(this, this);
		}
	}
}