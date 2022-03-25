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
	struct FDialogSelectionStruct GetSelectionInfo(); //was FORCEINLINE

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UDialogCue>> CueOptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bHasTimer = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimerDuration = -1.0f;

private:

	FTimerHandle SelectionEndTimer;

	class UDialog* OwningDialog;
	
};
