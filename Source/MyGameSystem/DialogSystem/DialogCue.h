// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogUnit.h"
#include "DialogCue.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogCue : public UDialogUnit
{
	GENERATED_BODY()

public:

	virtual void Activate_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "DialogCue|Control")
	void PlayNextDialogCue();

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogCue|Control")
	void OnCueBeginned();
	virtual void OnCueBeginned_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogCue|Control")
	void OnCueEnded();
	virtual void OnCueEnded_Implementation() { return; }

	UFUNCTION(BlueprintCallable, Category = "DialogCue|Internal")
	void EndCue();

protected:

	// ...

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogCue|DialogFlow", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UDialogUnit> NextDialogUnit = nullptr;

};
