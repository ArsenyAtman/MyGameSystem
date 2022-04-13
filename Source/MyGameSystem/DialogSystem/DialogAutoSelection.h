// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogSelection.h"
#include "DialogAutoSelectionDataAsset.h"
#include "DialogAutoSelection.generated.h"

UCLASS()
class MYGAMESYSTEM_API UDialogAutoSelection : public UDialogSelection
{
	GENERATED_BODY()

public:

	virtual void Activate_Implementation(class UDialog* OwnDialog) override;

	virtual class UDialogAutoSelectionDataAsset* GetDialogUnitData_Implementation() const override;

protected:

	// ...

private:

	// ...
	
};
