// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AxisInputHandler.h"
#include "AxisActionInputHandler.generated.h"

DECLARE_DELEGATE(FActionFunctionDelegate);

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UAxisActionInputHandler : public UAxisInputHandler
{
	GENERATED_BODY()

public:

	virtual void Bind_Implementation(class APlayerController* PlayerController, UObject* Object) override;

	virtual void Unbind_Implementation() override;

protected:
	
	virtual void HandleAxisInput_Implementation(float Value) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleReleaseAction();
	virtual void HandleReleaseAction_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, meta = (ExposeOnSpawn = true))
	FName ReleaseFunctionName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, meta = (ExposeOnSpawn = true))
	float ActionAxisValue = 0.5f;

private:

	FActionFunctionDelegate ReleaseActionDelegate;

	bool bWasReleased = true;
	
};
