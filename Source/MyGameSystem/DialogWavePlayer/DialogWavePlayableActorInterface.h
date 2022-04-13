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
 * 
 */
class MYGAMESYSTEM_API IDialogWavePlayableActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogWavePlayableActorInterface")
	class UDialogWavePlayerComponent* GetDialogWavePlayerComponent() const;
	virtual class UDialogWavePlayerComponent* GetDialogWaveComponent_Implementation() const = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogWavePlayableActorInterface")
	class USceneComponent* GetMouthComponent() const;
	virtual class USceneComponent* GetMouthComponent_Implementation() const = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogWavePlayableActorInterface")
	FName GetMouthPoint() const;
	virtual FName GetMouthPoint_Implementation() const { return "None"; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogWavePlayableActorInterface")
	FVector GetMouthRelativeLocation() const;
	virtual FVector GetMouthRelativeLocation_Implementation() const { return FVector(); }
};
