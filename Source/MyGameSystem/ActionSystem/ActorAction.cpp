// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorAction.h"
#include "ActionDriverComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UActorAction::StartAction()
{
	OnActionStarted();
}

void UActorAction::StartActionEvent()
{
	bEventStarted = true;
	OnEventStarted();
}

void UActorAction::EndActionEvent()
{
	bEventStarted = false;
	OnEventEnded();
}

void UActorAction::StartComboWindow()
{
	bComboWindowStarted = true;
	OnComboWindowStarted();
}

void UActorAction::EndComboWindow()
{
	bComboWindowStarted = false;
	OnComboWindowEnded();
}

void UActorAction::EndAction()
{
	if (bEventStarted)
	{
		EndActionEvent();
	}

	if (IsValid(GetActionDriverComponent()))
	{
		GetActionDriverComponent()->ActionCompleted(this);
	}

	OnActionEnded();
}

AActor* UActorAction::GetControlledActor() const
{
	if (IsValid(GetActionDriverComponent()))
	{
		return GetActionDriverComponent()->GetOwner();
	}
	return nullptr;
}

UActionDriverComponent* UActorAction::GetActionDriverComponent() const
{
	return Cast<UActionDriverComponent>(GetOuter());
}
