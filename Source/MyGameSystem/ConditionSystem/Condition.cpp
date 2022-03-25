// Fill out your copyright notice in the Description page of Project Settings.


#include "Condition.h"
#include "ConditionComponent.h"

void UCondition::StartCondition()
{
	OnConditionStarted();
}

AActor* UCondition::GetControlledActor()
{
	if (IsValid(GetConditionComponent()))
	{
		return GetConditionComponent()->GetOwner();
	}
	return nullptr;
}

UConditionComponent* UCondition::GetConditionComponent()
{
	if (IsValid(GetOuter()))
	{
		return Cast<UConditionComponent>(GetOuter());
	}
	return nullptr;
}

void UCondition::ChangeCondition(UCondition* NextCondition)
{
	if (IsValid(GetConditionComponent()) && IsValid(NextCondition))
	{
		GetConditionComponent()->ConditionChange(NextCondition);
		OnConditionEnded();
	}
}