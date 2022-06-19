// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Place.h"
#include "Container.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UContainer : public UPlace
{
	GENERATED_BODY()
	
public:

	virtual bool Add_Implementation(class UInventoryItem* Item, FPoint Location = FPoint()) override;

	virtual UInventoryItem* Remove_Implementation(class UInventoryItem* Item = nullptr) override;

	UFUNCTION(BlueprintPure)
	class UInventoryItem* GetLastItem();

	UFUNCTION(BlueprintPure)
	int Num();

	virtual TArray<class UInventoryItem*> GetItems_Implementation() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TArray<class UInventoryItem*> Items;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxCountOfItems;
};
