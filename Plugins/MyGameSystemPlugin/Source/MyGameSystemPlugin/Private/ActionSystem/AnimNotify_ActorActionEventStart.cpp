// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/AnimNotify_ActorActionEventStart.h"

#include "ActionSystem/ActorAction.h"
#include "ActionSystem/ActionDriverComponent.h"
#include "ActionSystem/ActionDrivenActorInterface.h"

void UAnimNotify_ActorActionEventStart::Notify(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation)
{
	UActorAction* ActorAction = GetActorAction(MeshComponent);
	if (IsValid(ActorAction))
	{
		ActorAction->StartActionEvent();
	}
}