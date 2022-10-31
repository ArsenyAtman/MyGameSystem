// Fill out your copyright notice in the Description page of Project Settings.


#include "ConditionSystem/ConditionComponent.h"

#include "ConditionSystem/Condition.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UConditionComponent::UConditionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// This component is replicated by default.
	SetIsReplicatedByDefault(true);
	
}

void UConditionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UConditionComponent, CurrentCondition);
}

void UConditionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Create an initial condition object of the initial condition class.
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(InitialConditionClass))
		{
			SetCurrentCondition(NewObject<UCondition>(this, InitialConditionClass));
		}
	}
}

void UConditionComponent::ConditionChange(UCondition* NewCondition)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(NewCondition))
		{
			NewCondition->ChangeOuter(this);
			SetCurrentCondition(NewCondition);
		}
	}
}

void UConditionComponent::SetCurrentCondition(UCondition* NewCondition)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		// Immediately destroy and end the current condition.
		if(IsValid(CurrentCondition))
		{
			CurrentCondition->Destroy();
		}

		// Set and start the new condition.
		CurrentCondition = NewCondition;
		CurrentCondition->StartCondition();

		// Notify about the change.
		OnConditionChanged.Broadcast();
	}
}

void UConditionComponent::OnRep_CurrentCondition()
{
	// Notify about the change.
	OnConditionChanged.Broadcast();
}