// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemLocator.h"
#include "ItemLocator_Grid.generated.h"

/**
 * ItemLocator for grid places.
 */
UCLASS()
class MYGAMESYSTEM_API UItemLocator_Grid : public UItemLocator
{
	GENERATED_BODY()

public:

	virtual FTransform GetItemRelativeTransform_Implementation(UItemPlace* OwningPlace, AItem* Item) const override;

	/**
	 * The step of the grid.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemLocator_Grid")
	float GridStepSize = 5.0f;

	/**
	 * Relative rotation for items.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemLocator_Grid")
	FRotator ItemsRelativeRotation;
	
};
