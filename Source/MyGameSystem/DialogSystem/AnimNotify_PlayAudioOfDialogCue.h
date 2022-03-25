// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayAudioOfDialogCue.generated.h"

UCLASS()
class MYGAMESYSTEM_API UAnimNotify_PlayAudioOfDialogCue : public UAnimNotify
{
	GENERATED_BODY()

protected:

	virtual void Notify(class USkeletalMeshComponent* MeshComponent, class UAnimSequenceBase* Animation) override;
	
};
