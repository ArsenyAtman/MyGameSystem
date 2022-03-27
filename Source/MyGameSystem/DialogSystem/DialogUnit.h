// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogUnit.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class MYGAMESYSTEM_API UDialogUnitInfo : public UDataAsset
{
	GENERATED_BODY()

};

UCLASS(BlueprintType, Blueprintable, Abstract)
class MYGAMESYSTEM_API UDialogUnit : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Activate(class UDialog* OwnDialog);
	virtual void Activate_Implementation(class UDialog* OwnDialog) { return; }

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	class UDialogUnitInfo* GetDialogUnitInfo();
	virtual class UDialogUnitInfo* GetDialogUnitInfo_Implementation() { return nullptr; }

private:

};
