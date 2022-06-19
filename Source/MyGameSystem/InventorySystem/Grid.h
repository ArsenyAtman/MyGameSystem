// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Place.h"
#include "Grid.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYGAMESYSTEM_API UGrid : public UPlace
{
	GENERATED_BODY()
	
public:

	virtual bool Add_Implementation(class UInventoryItem* Item, FPoint Location = FPoint()) override;

	virtual class UInventoryItem* Remove_Implementation(class UInventoryItem* Item = nullptr) override;

	virtual void DestroyItems_Implementation() override;

	virtual TArray<class UInventoryItem*> GetItems_Implementation() override;

	virtual FVector TransformLocation_Implementation(FPoint Location) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPoint Size;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Step = 0.0;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
	TArray<class UInventoryItem*> Items;
};
