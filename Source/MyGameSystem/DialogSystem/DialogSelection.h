// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogUnit.h"
#include "DialogSelection.generated.h"

/**
 * DialogUnit to controll the flow of a dialog.
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogSelection : public UDialogUnit
{
	GENERATED_BODY()

public:

	// Override activation.
	virtual void Activate_Implementation() override;

	/**
	 * Select the next dialog unit to play.
	 * @param CueIndex - The index of the next dialog unit to play.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogSelection|Control")
	void SelectNextCue(int CueIndex);
	virtual void SelectNextCue_Implementation(int CueIndex);

	/**
	 * Get the list of available options for the selection.
	 */
	UFUNCTION(BlueprintPure, Category = "DialogSelection|Data")
	TArray<TSubclassOf<class UDialogUnit>> GetAvailableOptions() const;

protected:

	/**
	 * Called after the start of this selection.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogSelection|Control", meta = (BlueprintProtected))
	void OnSelectionStarted();
	virtual void OnSelectionStarted_Implementation() { return; }

	/**
	 * Called after the end of this selection.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogSelection|Control", meta = (BlueprintProtected))
	void OnSelectionEnded();
	virtual void OnSelectionEnded_Implementation() { return; }

private:

	/**
	 * Options to choose from.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "DialogSelection|Options", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class UDialogUnit>> Options;
	
};
