// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify_DialogCue.h"
#include "AnimNotify_PlayNextDialogCue.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UAnimNotify_PlayNextDialogCue : public UAnimNotify_DialogCue
{
	GENERATED_BODY()

protected:

	virtual void Notify(class USkeletalMeshComponent* MeshComponent, class UAnimSequenceBase* Animation) override;
	
};
