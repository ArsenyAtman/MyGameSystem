// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "DialogUnit.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class MYGAMESYSTEM_API UDialogUnit : public UAdvancedObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogUnit|Internal")
	void Activate();
	virtual void Activate_Implementation() { return; }

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "DialogUnit|Availability")
	bool CheckAvailabilityCondition(class UDialog* CheckingDialog) const;
	virtual bool CheckAvailabilityCondition_Implementation(class UDialog* CheckingDialog) const { return true; }

	UFUNCTION(BlueprintPure, Category = "DialogUnit|OwningDialog")
	class UDialog* GetOwningDialog() const;

protected:

	// ...

private:

	// ...

};
