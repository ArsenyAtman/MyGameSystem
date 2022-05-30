// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AxisInputHandler.h"
#include "AxisActionInputHandler.generated.h"

/**
 * Delegate for axis action handlers.
 */
DECLARE_DELEGATE(FAxisActionFunctionDelegate);

/**
 * InpuHandler that handles axis events as action events (handle gamepad axis as buttons).
 */
UCLASS()
class MYGAMESYSTEM_API UAxisActionInputHandler : public UAxisInputHandler
{
	GENERATED_BODY()

public:

	// Bind this handler.
	virtual void Bind_Implementation(class APlayerController* PlayerController, UObject* Object) override;

	// Unbind this handler.
	virtual void Unbind_Implementation() override;

protected:
	
	// Handle the input.
	virtual void HandleAxisInput_Implementation(float Value) override;

	/**
	 * Handle the release event.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (BlueprintProtected))
	void HandleReleaseAction();
	virtual void HandleReleaseAction_Implementation();

	/**
	 * A name of the function to call when released.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, meta = (ExposeOnSpawn = true, BlueprintProtected))
	FName ReleaseFunctionName;

	/**
	 * The axis value on which to call the action event.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, meta = (ExposeOnSpawn = true, BlueprintProtected))
	float ActionAxisValue = 0.5f;

private:

	FAxisActionFunctionDelegate ReleaseActionDelegate;
	bool bWasReleased = true;
	
};
