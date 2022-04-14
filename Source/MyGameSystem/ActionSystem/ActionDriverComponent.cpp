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

	//...
}

void UActionDriverComponent::StartAction(UActorAction* Action)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Action) && Action->GetOuter() == this)
		{
			if (IsValid(GetCurrentAction()))
			{
				GetCurrentAction()->NewActionTriedToStart(Action);
			}
			else
			{
				SetCurrentAction(Action);
				GetCurrentAction()->StartAction();
				GetCurrentAction()->GetEventStarted();
				ActionStarted(GetCurrentActionClass());
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
			UClass* ActionClass = GetCurrentActionClass();
			SetCurrentAction(nullptr);
			ActionEnded(ActionClass);
		}
	}
}

void UActionDriverComponent::SetCurrentAction(class UActorAction* NewAction)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		CurrentAction = NewAction;
		if (IsValid(CurrentAction))
		{
			CurrentActionClass = CurrentAction->GetClass();
		}
		else
		{
			CurrentActionClass = nullptr;
		}
	}
}

void UActionDriverComponent::ActionStarted_Implementation(UClass* ActionClass)
{
	if (GetOwnerRole() != ENetRole::ROLE_Authority)
	{
		CurrentActionClass = ActionClass;
	}

	if (OnActionStarted.IsBound())
	{
		OnActionStarted.Broadcast();
	}
}

void UActionDriverComponent::ActionEnded_Implementation(UClass* ActionClass)
{
	if (GetOwnerRole() != ENetRole::ROLE_Authority)
	{
		CurrentActionClass = nullptr;
	}

	if (OnActionEnded.IsBound())
	{
		OnActionEnded.Broadcast(ActionClass);
	}
}
