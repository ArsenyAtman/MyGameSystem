// Fill out your copyright notice in the Description page of Project Settings.


#include "InputHandler.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

void UInputHandler::InputHandled()
{
	GetWorld()->GetTimerManager().SetTimer(InputPauseTimer, PauseDuration, false);
}

bool UInputHandler::GetCanHandleInput()
{
	return !(GetWorld()->GetTimerManager().IsTimerActive(InputPauseTimer));
}
