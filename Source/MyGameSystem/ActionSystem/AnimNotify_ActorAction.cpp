// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ActorAction.h"
#include "ActorAction.h"
#include "ActionDriverComponent.h"
#include "ActionDrivenActorInterface.h"

UActorAction* UAnimNotify_ActorAction::GetActorAction(USkeletalMeshComponent* MeshComponent)
{
	if (IsValid(MeshComponent))
	{
		AActor* Owner = MeshComponent->GetOwner();
		if (IsValid(Owner))
		{
			if (Owner->GetLocalRole() == ENetRole::ROLE_Authority)
			{
				if (Owner->Implements<UActionDrivenActorInterface>())
				{
					UActionDriverComponent* ActionDriver = IActionDrivenActorInterface::Execute_GetActionDriverComponent(Owner);
					if (IsValid(ActionDriver))
					{
						return ActionDriver->GetCurrentAction();
					}
				}
			}
		}
	}

	return nullptr;
}