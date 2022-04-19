// Fill out your copyright notice in the Description page of Project Settings.


#include "ConditionComponent.h"
#include "Condition.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UConditionComponent::UConditionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

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
		if (IsValid(NewCondition) && NewCondition->GetOuter() == this)
		{
			SetCurrentCondition(NewCondition);
		}
	}
}

void UConditionComponent::SetCurrentCondition(UCondition* NewCondition)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if(IsValid(CurrentCondition))
		{
			CurrentCondition->Destroy();
		}
		CurrentCondition = NewCondition;
		CurrentCondition->StartCondition();
		if (OnConditionChanged.IsBound())
		{
			OnConditionChanged.Broadcast();
		}
	}
}

void UConditionComponent::OnRep_CurrentCondition()
{
	if (OnConditionChanged.IsBound())
	{
		OnConditionChanged.Broadcast();
	}
}