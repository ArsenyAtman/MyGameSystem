// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogUnit.h"
#include "DialogAutoSelection.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UDialogAutoSelectionInfo : public UDialogUnitInfo
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

	virtual class UDialogAutoSelectionInfo* GetDialogUnitInfo_Implementation() override { return Info; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogAutoSelectionInfo* Info;
	
};
