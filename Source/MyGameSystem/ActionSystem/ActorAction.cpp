// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorAction.h"
#include "ActionDriverComponent.h"
#include "Net/UnrealNetwork.h"

void UActorAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UActorAction, bEventStarted);
	DOREPLIFETIME(UActorAction, bComboWindowStarted);
}

void UActorAction::StartAction()
{
	if(HasAuthority())
	{
		OnActionStarted();

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

		OnActionEnded();

		Notify_ActionEnd();
	}
}

void UActorAction::StartActionEvent()
{
	if(HasAuthority())
	{
		bEventStarted = true;
		OnEventStarted();
	}
}

void UActorAction::EndActionEvent()
{
	if(HasAuthority())
	{
		bEventStarted = false;
		OnEventEnded();
	}
}

void UActorAction::StartComboWindow()
{
	if(HasAuthority())
	{
		bComboWindowStarted = true;
		OnComboWindowStarted();
	}
}

void UActorAction::EndComboWindow()
{
	if(HasAuthority())
	{
		bComboWindowStarted = false;
		OnComboWindowEnded();
	}
}

void UActorAction::Notify_ActionStart_Implementation()
{
	Broadcast_ActionStart();
}

void UActorAction::Notify_ActionEnd_Implementation()
{
	Broadcast_ActionEnd();
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
