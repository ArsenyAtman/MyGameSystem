// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerWithInputHandlers.generated.h"

/**
 * Advanced player controller with an array of user input handlers.
 */
UCLASS()
class MYGAMESYSTEM_API APlayerControllerWithInputHandlers : public APlayerController
{
	GENERATED_BODY()

public:

	// Constructor
	APlayerControllerWithInputHandlers();

protected:

	// Override to bind all input handlers of this controller.
	virtual void SetupInputComponent() override;

	/**
	 * An array of input handlers.
	 */
	UPROPERTY(Instanced, EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (BlueprintProtected))
	TArray<class UInputHandler*> InputHandlers;
	
};
