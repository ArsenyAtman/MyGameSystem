// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify_ActorAction.h"
#include "AnimNotify_ActorActionComboWindowStart.generated.h"

/**
 * Notify the current action about the combo window start.
 */
UCLASS()
class MYGAMESYSTEMPLUGIN_API UAnimNotify_ActorActionComboWindowStart : public UAnimNotify_ActorAction
{
	GENERATED_BODY()

protected:

	virtual void Notify(class USkeletalMeshComponent* MeshComponent, class UAnimSequenceBase* Animation) override;
	
};
