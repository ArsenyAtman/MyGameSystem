// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogUnit.h"
#include "DialogAutoSelection.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UDialogAutoSelectionData : public UDialogUnitData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<class UDialogCue>> CueOptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCheckFromEnd = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bSelectFalseCondition = false;

};

UCLASS()
class MYGAMESYSTEM_API UDialogAutoSelection : public UDialogUnit
{
	GENERATED_BODY()

public:

	virtual void Activate_Implementation(class UDialog* OwnDialog) override;

	virtual class UDialogAutoSelectionData* GetDialogUnitData_Implementation() override { return Data; }

protected:

	// ...

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogAutoSelection|Data", meta = (AllowPrivateAccess = true))
	class UDialogAutoSelectionData* Data;
	
};
