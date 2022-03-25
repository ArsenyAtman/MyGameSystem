// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerWithInputHandlers.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API APlayerControllerWithInputHandlers : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerControllerWithInputHandlers();

protected:

	virtual void SetupInputComponent() override;

	UPROPERTY(Instanced, EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<class UInputHandler*> InputHandlers;
	
};
