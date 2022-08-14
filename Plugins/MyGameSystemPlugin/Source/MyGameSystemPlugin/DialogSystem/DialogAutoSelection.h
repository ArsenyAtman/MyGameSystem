// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogSelection.h"
#include "DialogAutoSelection.generated.h"

/**
 * DialogSelection that automatically selects the first available option.
 */
UCLASS()
class MYGAMESYSTEM_API UDialogAutoSelection : public UDialogSelection
{
	GENERATED_BODY()

public:

	// ...

protected:

	virtual void OnSelectionStarted_Implementation() override;

private:

	/**
	 * Should it check all options from the end of the list.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogAutoSelection|DialogFlow", meta = (AllowPrivateAccess = true))
	bool bCheckFromEnd = false;

	/**
	 * Should it pick the first unavailable option instead of the first available.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogAutoSelection|DialogFlow", meta = (AllowPrivateAccess = true))
	bool bSelectFalseCondition = false;
	
};
