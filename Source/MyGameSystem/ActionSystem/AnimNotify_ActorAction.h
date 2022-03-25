// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ActorAction.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UAnimNotify_ActorAction : public UAnimNotify
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
	class UActorAction* GetActorAction(class USkeletalMeshComponent* MeshComponent);
	
};
