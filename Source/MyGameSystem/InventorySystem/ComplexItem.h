// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "StorageInterface.h"
#include "ComplexStorageInterface.h"
#include "ComplexItem.generated.h"

class UItemPlace;

/**
 * Item with subplaces.
 */
UCLASS()
class MYGAMESYSTEM_API AComplexItem : public AItem, public IStorageInterface, public IComplexStorageInterface
{
	GENERATED_BODY()

public:

	// Instanced interface.
	virtual void SetIsInstanced_Implementation(bool bNewIsInstanced) override;

	// Storage interface.
	virtual bool AddItem_Implementation(AItem* Item);
	virtual TArray<AItem*> FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const;
	virtual bool CheckItemPossession_Implementation(AItem* Item) const;
	
	// Complex storage interface.
	virtual TArray<UItemPlace*> GetPlaces_Implementation() const;
	
};
