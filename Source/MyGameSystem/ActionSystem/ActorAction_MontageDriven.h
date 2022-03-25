// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorAction.h"
#include "ActorAction_MontageDriven.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UActorAction_MontageDriven : public UActorAction
{
	GENERATED_BODY()

protected:

	virtual void OnActionStarted_Implementation() override;

	virtual void OnActionEnded_Implementation() override;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	class UAnimMontage* GetAnimationMontageToPlay();
	virtual class UAnimMontage* GetAnimationMontageToPlay_Implementation() { return nullptr; }

	UFUNCTION()
	void AnimationPlayed(class UAnimMontage* AnimMontage, bool bInterrupted);

private:

	class UAnimInstance* PlayingAnimInstance;

	class UAnimMontage* PlayingAnimMontage;
	
};
