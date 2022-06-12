// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogSelection.h"
#include "DialogAutoSelection.generated.h"

UCLASS()
class MYGAMESYSTEM_API UDialogAutoSelection : public UDialogSelection
{
	GENERATED_BODY()

public:

	// ...

protected:

	virtual void OnSelectionStarted_Implementation() override;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogAutoSelection|DialogFlow", meta = (AllowPrivateAccess = true))
	bool bCheckFromEnd = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogAutoSelection|DialogFlow", meta = (AllowPrivateAccess = true))
	bool bSelectFalseCondition = false;
	
};
