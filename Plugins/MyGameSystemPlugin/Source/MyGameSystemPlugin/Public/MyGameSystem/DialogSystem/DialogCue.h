// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogUnit.h"
#include "DialogCue.generated.h"

/**
 * DialogUnit that represents some dialog cue.
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEMPLUGIN_API UDialogCue : public UDialogUnit
{
	GENERATED_BODY()

public:

	// Override the activation.
	virtual void Activate_Implementation() override;

	/**
	 * Play the next dialog cue.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogCue|Control")
	void PlayNextDialogCue();

	// TODO: Skip this dialog cue.

	// TODO: bWasFired.

protected:

	/**
	 * Called after the cue start.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogCue|Control", meta = (BlueprintProtected))
	void OnCueBeginned();
	virtual void OnCueBeginned_Implementation() { return; }

	/**
	 * Called before the cue end.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogCue|Control", meta = (BlueprintProtected))
	void OnCueEnded();
	virtual void OnCueEnded_Implementation() { return; }

	/**
	 * End this cue.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogCue|Internal", meta = (BlueprintProtected))
	void EndCue();

private:

	/**
	 * The next dialog cue to play.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogCue|DialogFlow", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UDialogUnit> NextDialogUnit = nullptr;

};
