// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogUnit.h"
#include "DialogAutoSelectionDataAsset.h"
#include "DialogAutoSelection.generated.h"

UCLASS()
class MYGAMESYSTEM_API UDialogAutoSelection : public UDialogUnit
{
	GENERATED_BODY()

public:

	virtual void Activate_Implementation(class UDialog* OwnDialog) override;

	virtual class UDialogAutoSelectionDataAsset* GetDialogUnitData_Implementation() override { return Data; }

protected:

	// ...

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogAutoSelection|Data", meta = (AllowPrivateAccess = true))
	class UDialogAutoSelectionDataAsset* Data;
	
};
