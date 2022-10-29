// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/AnimNotify_ActorAction.h"

#include "ActionSystem/ActorAction.h"
#include "ActionSystem/ActionDriverComponent.h"
#include "ActionSystem/ActionDrivenActorInterface.h"

UActorAction* UAnimNotify_ActorAction::GetActorAction(const USkeletalMeshComponent* MeshComponent) const
{
	if (IsValid(MeshComponent))
	{
		AActor* Owner = MeshComponent->GetOwner();
		if (IsValid(Owner) && Owner->GetLocalRole() == ENetRole::ROLE_Authority && Owner->Implements<UActionDrivenActorInterface>())
		{
			UActionDriverComponent* ActionDriver = IActionDrivenActorInterface::Execute_GetActionDriverComponent(Owner);
			if (IsValid(ActionDriver))
			{
				return ActionDriver->GetCurrentAction();
			}
		}
	}

	return nullptr;
}