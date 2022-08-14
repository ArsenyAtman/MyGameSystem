// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputHandler.h"
#include "ActionInputHandler.generated.h"

/**
 * Delegate for acton input handlers.
 */
DECLARE_DELEGATE(FActionFunctionDelegate);

/**
 * InputHandlers for the action input type.
 */
UCLASS()
class MYGAMESYSTEMPLUGIN_API UActionInputHandler : public UInputHandler
{
	GENERATED_BODY()

public:

	// Bind this handler.
	virtual void Bind_Implementation(class APlayerController* PlayerController, class UObject* Object) override;

	// Unbind this handler.
	virtual void Unbind_Implementation() override;

protected:

	/**
	 * Determines which input to respond to this handler
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, meta = (ExposeOnSpawn = true, BlueprintProtected))
	TEnumAsByte<EInputEvent> KeyEvent;

	/**
	 * Handle the input.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (BlueprintProtected))
	void HandleActionInput();
	virtual void HandleActionInput_Implementation();

private:

	FActionFunctionDelegate ActionDelegate;
	class APlayerController* RelatedPlayerController;
	FInputActionBinding InputActionBinding;
	
};
