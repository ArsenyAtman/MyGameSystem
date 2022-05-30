// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputHandler.h"
#include "AxisInputHandler.generated.h"

/**
 * Delegate for axis input handlers.
 */
DECLARE_DELEGATE_OneParam(FAxisFunctionDelegate, float);

/**
 * InputHandler for the axis input type.
 */
UCLASS()
class MYGAMESYSTEM_API UAxisInputHandler : public UInputHandler
{
	GENERATED_BODY()

public:

	// Bind this handler.
	virtual void Bind_Implementation(class APlayerController* PlayerController, UObject* Object) override;

	// Unbind this handler.
	virtual void Unbind_Implementation() override;

protected:

	/**
	 * Handle the axis input.
	 * @param Value - An input value of the input axis.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (BlueprintProtected))
	void HandleAxisInput(float Value);
	virtual void HandleAxisInput_Implementation(float Value);

private:

	FAxisFunctionDelegate AxisDelegate;
	class APlayerController* RelatedPlayerController;
	FInputAxisBinding InputAxisBinding;
	
};
