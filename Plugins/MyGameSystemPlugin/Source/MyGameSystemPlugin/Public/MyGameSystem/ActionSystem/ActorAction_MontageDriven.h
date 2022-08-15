// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorAction.h"
#include "ActorAction_MontageDriven.generated.h"

/**
 * Action class with the ability to play an AnimMontage on the controlled actor.
 */
UCLASS()
class MYGAMESYSTEMPLUGIN_API UActorAction_MontageDriven : public UActorAction
{
	GENERATED_BODY()

protected:

	// Override for the animation setup.
	virtual void OnActionStarted_Implementation() override;

	// Override for the animation clean up.
	virtual void OnActionEnded_Implementation() override;

	/**
	 * Determine which animation to play.
	 * @return AnimMontage for playing.
	 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	class UAnimMontage* GetAnimationMontageToPlay() const;
	virtual class UAnimMontage* GetAnimationMontageToPlay_Implementation() const { return nullptr; }

	/**
	 * Called when the playing animation is done.
	 */
	UFUNCTION()
	void AnimationPlayed(class UAnimMontage* AnimMontage, bool bInterrupted);

private:

	// The playing animation instance.
	class UAnimInstance* PlayingAnimInstance;

	// The playing animation montage.
	class UAnimMontage* PlayingAnimMontage;
	
};
