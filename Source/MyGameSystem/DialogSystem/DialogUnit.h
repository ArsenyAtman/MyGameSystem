// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogUnit.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class MYGAMESYSTEM_API UDialogUnitData : public UDataAsset
{
	GENERATED_BODY()

};

UCLASS(BlueprintType, Blueprintable, Abstract)
class MYGAMESYSTEM_API UDialogUnit : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogUnit|Internal")
	void Activate(class UDialog* OwnDialog);
	virtual void Activate_Implementation(class UDialog* OwnDialog) { return; }

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "DialogUnit|Data")
	class UDialogUnitData* GetDialogUnitData();
	virtual class UDialogUnitData* GetDialogUnitData_Implementation() { return nullptr; }

private:

};
