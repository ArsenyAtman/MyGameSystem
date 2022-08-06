// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemLocator.h"
#include "ItemLocator_Grid.generated.h"


UCLASS()
class MYGAMESYSTEM_API UItemLocator_Grid : public UItemLocator
{
	GENERATED_BODY()

public:

	virtual FTransform GetItemRelativeTransform_Implementation(UItemPlace* OwningPlace, AItem* Item) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float GridStepSize = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator ItemsRelativeRotation;
	
};
