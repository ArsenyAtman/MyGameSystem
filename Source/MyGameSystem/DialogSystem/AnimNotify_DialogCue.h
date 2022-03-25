// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_DialogCue.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UAnimNotify_DialogCue : public UAnimNotify
{
	GENERATED_BODY()

protected:

	virtual class UDialogCue* GetCurrentDialogCue(class USkeletalMeshComponent* MeshComponent);
	
};
