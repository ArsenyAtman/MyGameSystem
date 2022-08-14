// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify_DialogCue.h"
#include "AnimNotify_PlayAudioOfDialogCue.generated.h"

/**
 * Starts an audio of the current dialog cue.
 */
UCLASS()
class MYGAMESYSTEM_API UAnimNotify_PlayAudioOfDialogCue : public UAnimNotify_DialogCue
{
	GENERATED_BODY()

protected:

	virtual void Notify(class USkeletalMeshComponent* MeshComponent, class UAnimSequenceBase* Animation) override;
	
};
