// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ActorAction.h"
#include "ActorAction.h"
#include "ActionDriverComponent.h"
#include "ActionDrivenActorInterface.h"

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