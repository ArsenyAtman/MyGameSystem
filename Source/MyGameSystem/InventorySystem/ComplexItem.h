// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/InventorySystem/Item.h"
#include "StorageInterface.h"
#include "ComplexItem.generated.h"


class UItemPlace;

UCLASS()
class MYGAMESYSTEM_API UComplexItem : public UItem, public IStorageInterface
{
	GENERATED_BODY()

public:

	virtual TArray<UItemPlace*> GetItemPlaces_Implementation() const override { return TArray<UItemPlace*>(); }
	
};
