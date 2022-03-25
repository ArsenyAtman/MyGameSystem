// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputHandler.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Blueprintable, Abstract)
class MYGAMESYSTEM_API UInputHandler : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Bind(class APlayerController* PlayerController, class UObject* Object);
	virtual void Bind_Implementation(class APlayerController* PlayerController, class UObject* Object) { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Unbind();
	virtual void Unbind_Implementation() { return; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, meta = (ExposeOnSpawn = true))
	FName InputMappingName = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, meta = (ExposeOnSpawn = true))
	FName RelatedFunctionName = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, meta = (ExposeOnSpawn = true))
	float PauseDuration = 0.1f;

	UFUNCTION(BlueprintCallable)
	void InputHandled();

	UFUNCTION(BlueprintPure)
	bool GetCanHandleInput();

private:

	FTimerHandle InputPauseTimer;
};
