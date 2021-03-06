// Fill out your copyright notice in the Description page of Project Settings.


#include "Condition.h"
#include "ConditionComponent.h"

void UCondition::StartCondition()
{
	if(HasAuthority())
	{
		ConditionStarted();

		Notify_ConditionStart();
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

void UCondition::EndPlay_Implementation()
{
	Broadcast_ConditionEnd();

	Super::EndPlay_Implementation();
}

void UCondition::ChangeCondition(UCondition* NextCondition)
{
	if(HasAuthority())
	{
		if (IsValid(GetConditionComponent()) && IsValid(NextCondition))
		{
			GetConditionComponent()->ConditionChange(NextCondition);

			ConditionEnded();
		}
	}
}

void UCondition::Notify_ConditionStart_Implementation()
{
	Broadcast_ConditionStart();
}

void UCondition::Broadcast_ConditionStart()
{
	OnConditionStarted.Broadcast(this);
}

void UCondition::Broadcast_ConditionEnd()
{
	OnConditionEnded.Broadcast(this);
}
