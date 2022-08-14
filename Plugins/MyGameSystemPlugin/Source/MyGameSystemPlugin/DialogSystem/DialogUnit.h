// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "DialogUnit.generated.h"

/**
 * Base class for elements (cues, selections, etc.) of a dialog.
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class MYGAMESYSTEM_API UDialogUnit : public UAdvancedObject
{
	GENERATED_BODY()

public:

	/**
	 * Activate this unit.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogUnit|Internal")
	void Activate();
	virtual void Activate_Implementation() { return; }

	/**
	 * Whether or not this unit is available.
	 * @param CheckingDialog - A dialog that checks this unit.
	 * @return IsAvailable.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "DialogUnit|Availability")
	bool CheckAvailabilityCondition(class UDialog* CheckingDialog) const;
	virtual bool CheckAvailabilityCondition_Implementation(class UDialog* CheckingDialog) const { return true; }

	/**
	 * Get the owning dialog of this unit.
	 * @return The owning dialog.
	 */
	UFUNCTION(BlueprintPure, Category = "DialogUnit|OwningDialog")
	class UDialog* GetOwningDialog() const;

protected:

	// ...

private:

	// ...

};
