// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputHandler.generated.h"

/**
 * A base class for user input handlers with a spam-protection.
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Blueprintable, Abstract)
class MYGAMESYSTEM_API UInputHandler : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * Bind this handler.
	 * @param PlayerController - A controller with an input event to bind.
	 * @param Object - An object with a function to call.
	 * @see InputMappingName & RelatedFunctionName.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Bind(class APlayerController* PlayerController, class UObject* Object);
	virtual void Bind_Implementation(class APlayerController* PlayerController, class UObject* Object) { return; }

	/**
	 * Unbind this handler.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Unbind();
	virtual void Unbind_Implementation() { return; }

protected:

	/**
	 * Input mapping name.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, meta = (ExposeOnSpawn = true, BlueprintProtected))
	FName InputMappingName = "";

	/**
	 * Name of the function to call.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, meta = (ExposeOnSpawn = true, BlueprintProtected))
	FName RelatedFunctionName = "";

	/**
	 * Delay after each call to protect this handler against input event spam.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, meta = (ExposeOnSpawn = true, BlueprintProtected))
	float PauseDuration = 0.1f;

	/**
	 * Called after the input event has been processed.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	void InputHandled();

	/**
	 * Is the input pause currently active.
	 * @return Can handle the input event call.
	 */
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected))
	bool GetCanHandleInput();

private:

	// Timer for the spam protection.
	FTimerHandle InputPauseTimer;
};
