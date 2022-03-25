// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ActorActionComboWindowStart.h"
#include "ActorAction.h"
#include "ActionDriverComponent.h"
#include "ActionDrivenActorInterface.h"

void UAnimNotify_ActorActionComboWindowStart::Notify(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation)
{
	UActorAction* ActorAction = GetActorAction(MeshComponent);
	if (IsValid(ActorAction))
	{
		ActorAction->StartComboWindow();
	}
}
