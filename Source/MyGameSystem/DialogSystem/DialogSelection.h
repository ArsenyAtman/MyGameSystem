// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogUnit.h"
#include "DialogSelectionDataAsset.h"
#include "DialogSelection.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogSelection : public UDialogUnit
{
	GENERATED_BODY()

public:

	virtual void Activate_Implementation(class UDialog* OwnDialog) override;

	virtual class UDialogSelectionDataAsset* GetDialogUnitData_Implementation() const override { return SelectionData; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogSelection|Internal")
	void SelectNextCue(int CueIndex);
	virtual void SelectNextCue_Implementation(int CueIndex);

	UFUNCTION(BlueprintPure, Category = "DialogSelection|Data")
	TArray<TSubclassOf<class UDialogCue>> GetAvailableOptions() const;

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSelectionStarted();
	virtual void OnSelectionStarted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSelectionEnded();
	virtual void OnSelectionEnded_Implementation() { return; }

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<class UDialogCue>> Options;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogSelection|Data", meta = (AllowPrivateAccess = true))
	class UDialogSelectionDataAsset* SelectionData;

	class UDialog* OwningDialog;
	
};
