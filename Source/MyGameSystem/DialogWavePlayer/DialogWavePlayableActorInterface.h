// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialogWavePlayableActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDialogWavePlayableActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for an actor that can play dialog waves.
 */
class MYGAMESYSTEM_API IDialogWavePlayableActorInterface
{
	GENERATED_BODY()

public:

	/**
	 * Get a dialog player component of this actor.
	 * @return The DialogWavePlayer.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogWavePlayableActorInterface")
	class UDialogWavePlayerComponent* GetDialogWavePlayerComponent() const;
	virtual class UDialogWavePlayerComponent* GetDialogWaveComponent_Implementation() const = 0;

	/**
	 * Get a component for attaching dialog waves.
	 * @return The mouth component.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogWavePlayableActorInterface")
	class USceneComponent* GetMouthComponent() const;
	virtual class USceneComponent* GetMouthComponent_Implementation() const = 0;

	/**
	 * Get a point of the mouth component to attach dialog waves to.
	 * @return The mouth point name.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogWavePlayableActorInterface")
	FName GetMouthPoint() const;
	virtual FName GetMouthPoint_Implementation() const { return "None"; }

	/**
	 * Get a correction of the mouth point position.
	 * @return The correction.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogWavePlayableActorInterface")
	FVector GetMouthRelativeLocation() const;
	virtual FVector GetMouthRelativeLocation_Implementation() const { return FVector(); }
};
