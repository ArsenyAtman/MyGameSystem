// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogUnit.h"
#include "DialogSelection.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UDialogSelectionData : public UDialogUnitData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<class UDialogCue>> Options;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bWithTimer = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Time = -1.0f;
};

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogSelection : public UDialogUnit
{
	GENERATED_BODY()

public:

	virtual void Activate_Implementation(class UDialog* OwnDialog) override;

	virtual class UDialogSelectionData* GetDialogUnitData_Implementation() override { return SelectionData; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogSelection|Internal")
	void SelectNextCue(int CueIndex);
	virtual void SelectNextCue_Implementation(int CueIndex);

	UFUNCTION(BlueprintPure, Category = "DialogSelection|Data")
	TArray<TSubclassOf<class UDialogCue>> GetAvailableOptions();

protected:

	// ...

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogSelection|Data", meta = (AllowPrivateAccess = true))
	class UDialogSelectionData* SelectionData;

	FTimerHandle SelectionEndTimer;

	class UDialog* OwningDialog;
	
};
