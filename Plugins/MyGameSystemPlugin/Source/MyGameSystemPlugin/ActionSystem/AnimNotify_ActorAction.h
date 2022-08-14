// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ActorAction.generated.h"

/**
 * Base class for the all AnimNotifies that control ActorActions.
 */
UCLASS()
class MYGAMESYSTEM_API UAnimNotify_ActorAction : public UAnimNotify
{
	GENERATED_BODY()

protected:

	/**
	 * Get the current actor action (only if it has an authority!).
	 * @return The current actor action.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	class UActorAction* GetActorAction(const class USkeletalMeshComponent* MeshComponent) const;
	
};
