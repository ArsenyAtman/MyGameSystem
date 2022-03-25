// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputHandler.h"
#include "ActionInputHandler.generated.h"

DECLARE_DELEGATE(FActionFunctionDelegate);

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UActionInputHandler : public UInputHandler
{
	GENERATED_BODY()

public:

	virtual void Bind_Implementation(class APlayerController* PlayerController, class UObject* Object) override;

	virtual void Unbind_Implementation() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, meta = (ExposeOnSpawn = true))
	TEnumAsByte<EInputEvent> KeyEvent;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleActionInput();
	virtual void HandleActionInput_Implementation();

private:

	FActionFunctionDelegate ActionDelegate;
	class APlayerController* RelatedPlayerController;
	FInputActionBinding InputActionBinding;
	
};
