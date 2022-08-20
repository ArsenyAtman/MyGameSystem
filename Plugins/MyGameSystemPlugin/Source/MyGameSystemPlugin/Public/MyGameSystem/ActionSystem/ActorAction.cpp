// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorAction.h"
#include "ActionDriverComponent.h"
#include "Net/UnrealNetwork.h"

void UActorAction::StartAction()
{
	if(HasAuthority())
	{
		Notify_ActionStart();
	}
}

void UActorAction::EndAction()
{
	if(HasAuthority())
	{
		if (bEventStarted)
		{
			EndActionEvent();
		}

		if (IsValid(GetActionDriverComponent()))
		{
			GetActionDriverComponent()->ActionCompleted(this);
		}

		Notify_ActionEnd();
	}
}

void UActorAction::StartActionEvent()
{
	if(HasAuthority())
	{
		Notify_ActionEventStart();
	}
}

void UActorAction::EndActionEvent()
{
	if(HasAuthority())
	{
		Notify_ActionEventEnd();
	}
}

void UActorAction::StartComboWindow()
{
	if(HasAuthority())
	{
		Notify_ComboWindowStart();
	}
}

void UActorAction::EndComboWindow()
{
	if(HasAuthority())
	{
		Notify_ComboWindowEnd();
	}
}

void UActorAction::Notify_ActionStart_Implementation()
{
	OnActionStarted();
	Broadcast_ActionStart();
}

void UActorAction::Notify_ActionEnd_Implementation()
{
	OnActionEnded();
	Broadcast_ActionEnd();
}

void UActorAction::Notify_ActionEventStart_Implementation()
{
	bEventStarted = true;
	OnEventStarted();
}

void UActorAction::Notify_ActionEventEnd_Implementation()
{
	bEventStarted = false;
	OnEventEnded();
}

void UActorAction::Notify_ComboWindowStart_Implementation()
{
	bComboWindowStarted = true;
	OnComboWindowStarted();
}

void UActorAction::Notify_ComboWindowEnd_Implementation()
{
	bComboWindowStarted = false;
	OnComboWindowEnded();
}

void UActorAction::Broadcast_ActionStart()
{
	OnActorActionStarted.Broadcast(this);
}

void UActorAction::Broadcast_ActionEnd()
{
	OnActorActionEnded.Broadcast(this);
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
