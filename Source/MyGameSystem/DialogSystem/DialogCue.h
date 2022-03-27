// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogUnitInterface.h"
#include "Delegates/IDelegateInstance.h"
#include "DialogSystemTypes.h"
#include "DialogCue.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogCue : public UObject, public IDialogUnitInterface
{
	GENERATED_BODY()

public:

	virtual void Activate_Implementation(class UDialog* OwnDialog) override;

	UFUNCTION(BlueprintCallable)
	void PlayNextDialogCue();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CheckAvailabilityCondition(class UDialog* CheckingDialog);
	virtual bool CheckAvailabilityCondition_Implementation(class UDialog* CheckingDialog);

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UDialogCueInfo* GetCueInfo() { return CueInfo; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UDialog* GetOwningDialog() { return OwningDialog; }

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnCueBeginned();
	virtual void OnCueBeginned_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnCueEnded();
	virtual void OnCueEnded_Implementation();

	UFUNCTION(BlueprintCallable)
	void EndCue();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MustImplement = "DialogUnitInterface"))
	TSubclassOf<UObject> NextDialogUnit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDialogCueType DialogCueType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogCueInfo* CueInfo;

private:

	class UDialog* OwningDialog;

};
