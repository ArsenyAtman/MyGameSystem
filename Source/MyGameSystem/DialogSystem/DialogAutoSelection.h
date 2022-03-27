// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogUnit.h"
#include "DialogAutoSelection.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UDialogAutoSelection : public UDialogUnit
{
	GENERATED_BODY()

public:

	virtual void Activate_Implementation(class UDialog* OwnDialog) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCheckFromEnd = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bSelectFalseCondition = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MustImplement = "DialogUnitInterface"))
	TArray<TSubclassOf<UObject>> CueOptions;
	
};
