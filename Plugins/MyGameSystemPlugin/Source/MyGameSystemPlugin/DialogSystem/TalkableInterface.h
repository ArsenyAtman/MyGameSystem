// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TalkableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTalkableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors with a DialogComponent.
 */
class MYGAMESYSTEM_API ITalkableInterface
{
	GENERATED_BODY()

public:

	/**
	 * Get the dialog component of this actor.
	 * @return The dialog component.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TalkableInterface")
	class UDialogComponent* GetDialogComponent() const;
	virtual class UDialogComponent* GetDialogComponent_Implementation() const = 0;

};
