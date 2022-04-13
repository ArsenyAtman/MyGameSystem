// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogUnit.h"
#include "DialogCueDataAsset.h"
#include "DialogSystemTypes.h"
#include "DialogCue.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogCue : public UDialogUnit
{
	GENERATED_BODY()

public:

	virtual void Activate_Implementation(class UDialog* OwnDialog) override;

	virtual class UDialogCueDataAsset* GetDialogUnitData_Implementation() const override { return CueData; }

	UFUNCTION(BlueprintCallable, Category = "DialogCue|Control")
	void PlayNextDialogCue();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogCue|Availability")
	bool CheckAvailabilityCondition(class UDialog* CheckingDialog) const;
	virtual bool CheckAvailabilityCondition_Implementation(class UDialog* CheckingDialog) const { return true; }

	UFUNCTION(BlueprintGetter, Category = "DialogCue|Dialog")
	class UDialog* GetOwningDialog() const { return OwningDialog; }

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogCue|Control")
	void OnCueBeginned();
	virtual void OnCueBeginned_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogCue|Control")
	void OnCueEnded();
	virtual void OnCueEnded_Implementation();

	UFUNCTION(BlueprintCallable, Category = "DialogCue|Internal")
	void EndCue();

protected:

	// ...

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogCue|DialogFlow", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UDialogUnit> NextDialogUnit = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogCue|Data", meta = (AllowPrivateAccess = true))
	class UDialogCueDataAsset* CueData;

	UPROPERTY(BlueprintGetter = GetOwningDialog)
	class UDialog* OwningDialog;

};
