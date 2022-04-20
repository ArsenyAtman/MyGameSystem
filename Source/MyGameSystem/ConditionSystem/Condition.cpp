// Fill out your copyright notice in the Description page of Project Settings.


#include "Condition.h"
#include "ConditionComponent.h"

void UCondition::StartCondition()
{
	if(HasAuthority())
	{
		OnConditionStarted();
	}
}

AActor* UCondition::GetControlledActor() const
{
	if (IsValid(GetConditionComponent()))
	{
		return GetConditionComponent()->GetOwner();
	}
	return nullptr;
}

UConditionComponent* UCondition::GetConditionComponent() const
{
	if (IsValid(GetOuter()))
	{
		return Cast<UConditionComponent>(GetOuter());
	}
	return nullptr;
}

void UCondition::ChangeCondition(UCondition* NextCondition)
{
	if(HasAuthority())
	{
		if (IsValid(GetConditionComponent()) && IsValid(NextCondition))
		{
			GetConditionComponent()->ConditionChange(NextCondition);
			OnConditionEnded();
		}
	}
}