// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogCue.h"
#include "DialogUnitInterface.h"
#include "DialogSelection.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogSelection : public UObject, public IDialogUnitInterface
{
	GENERATED_BODY()

public:

	virtual void Activate_Implementation(class UDialog* OwnDialog) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SelectNextCue(int CueIndex);
	virtual void SelectNextCue_Implementation(int CueIndex);

	UFUNCTION(BlueprintPure)
	TArray<TSubclassOf<UDialogCue>> GetAvailableOptions();

	UFUNCTION(BlueprintPure)
	class UDialogSelectionInfo* GetSelectionInfo() { return SelectionInfo; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogSelectionInfo* SelectionInfo;

private:

	FTimerHandle SelectionEndTimer;

	class UDialog* OwningDialog;
	
};
