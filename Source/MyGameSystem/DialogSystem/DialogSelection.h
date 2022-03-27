// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogUnit.h"
#include "DialogSelection.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UDialogSelectionInfo : public UDialogUnitInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MustImplement = "DialogUnitInterface"))
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

	virtual class UDialogSelectionInfo* GetDialogUnitInfo_Implementation() override { return SelectionInfo; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SelectNextCue(int CueIndex);
	virtual void SelectNextCue_Implementation(int CueIndex);

	UFUNCTION(BlueprintPure)
	TArray<TSubclassOf<class UDialogCue>> GetAvailableOptions();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogSelectionInfo* SelectionInfo;

private:

	FTimerHandle SelectionEndTimer;

	class UDialog* OwningDialog;
	
};
