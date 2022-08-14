// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_DialogCue.generated.h"

/**
 * Base class for AnimNotidies of the DialogSystem.
 */
UCLASS()
class MYGAMESYSTEMPLUGIN_API UAnimNotify_DialogCue : public UAnimNotify
{
	GENERATED_BODY()

protected:

	// Gets the current dialog cue.
	virtual class UDialogCue_AnimationAndSound* GetCurrentDialogCue(const class USkeletalMeshComponent* MeshComponent) const;
	
};
