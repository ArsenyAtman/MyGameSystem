// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogUnit.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class MYGAMESYSTEM_API UDialogUnit : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogUnit|Internal")
	void Activate(class UDialog* OwnDialog);
	virtual void Activate_Implementation(class UDialog* OwnDialog) { return; }

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "DialogUnit|Data")
	class UDialogUnitDataAsset* GetDialogUnitData() const;
	virtual class UDialogUnitDataAsset* GetDialogUnitData_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "DialogUnit|Availability")
	bool CheckAvailabilityCondition(class UDialog* CheckingDialog) const;
	virtual bool CheckAvailabilityCondition_Implementation(class UDialog* CheckingDialog) const { return true; }

private:

};
