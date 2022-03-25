// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputHandler.h"
#include "AxisInputHandler.generated.h"

DECLARE_DELEGATE_OneParam(FAxisFunctionDelegate, float);

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UAxisInputHandler : public UInputHandler
{
	GENERATED_BODY()

public:

	virtual void Bind_Implementation(class APlayerController* PlayerController, UObject* Object) override;

	virtual void Unbind_Implementation() override;

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleAxisInput(float Value);
	virtual void HandleAxisInput_Implementation(float Value);

private:

	FAxisFunctionDelegate AxisDelegate;
	class APlayerController* RelatedPlayerController;
	FInputAxisBinding InputAxisBinding;
	
};
