// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionDriverComponent.h"
#include "Net/UnrealNetwork.h"
#include "ActionDrivenActorInterface.h"
#include "ActorAction.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UActionDriverComponent::UActionDriverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	
}

void UActionDriverComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UActionDriverComponent, CurrentAction);
}

void UActionDriverComponent::StartAction(UActorAction* Action)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Action))
		{
			// Set this action driver as outer for the new action.
			Action->ChangeOuter(this);

			// If the current action is valid and running...
			if (IsValid(GetCurrentAction()))
			{
				// than notify it about the new action.
				GetCurrentAction()->NewActionTriedToStart(Action);
			}
			else
			{
				// else set the new action as current.
				SetCurrentAction(Action);
				GetCurrentAction()->StartAction();
			}
		}
	}
}

void UActionDriverComponent::AbortAction()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(GetCurrentAction()))
		{
			GetCurrentAction()->EndAction();
		}
	}
}

void UActionDriverComponent::ActionCompleted(UActorAction* Action)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (Action == GetCurrentAction())
		{
			// Now there is no current action.
			SetCurrentAction(nullptr);
		}
	}
}

void UActionDriverComponent::SetCurrentAction(class UActorAction* NewAction)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		CurrentAction = NewAction;
	}
}

void UActionDriverComponent::OnRep_CurrentAction()
{
	if(IsValid(CurrentAction))
	{
		OnActionStarted.Broadcast();
	}
	else
	{
		OnActionEnded.Broadcast();
	}
}
