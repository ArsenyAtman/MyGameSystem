// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "StorageInterface.h"
#include "ComplexStorageInterface.h"
#include "ComplexItem.generated.h"


class UItemPlace;

UCLASS()
class MYGAMESYSTEM_API AComplexItem : public AItem, public IStorageInterface, public IComplexStorageInterface
{
	GENERATED_BODY()

public:

	virtual void Instance_Implementation() override;
	virtual void Uninstance_Implementation() override;

	virtual bool AddItem_Implementation(AItem* Item);
	virtual TArray<AItem*> FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const;
	
	virtual TArray<UItemPlace*> GetPlaces_Implementation() const;
	
};
