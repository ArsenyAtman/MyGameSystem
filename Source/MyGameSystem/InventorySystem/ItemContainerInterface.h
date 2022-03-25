// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Item.h"
#include "ItemContainerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemContainerInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FUpdateDelegate);

/**
 *
 */
class MYGAMESYSTEM_API IItemContainerInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<struct FItemStruct> GetItems() const;
	virtual TArray<struct FItemStruct> GetItems_Implementation() const = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool AddItem(struct FItemStruct Item, int InPlace = -1);
	virtual bool AddItem_Implementation(struct FItemStruct Item, int InPlace = -1) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	struct FItemStruct RemoveItem(int FromPlace);
	virtual struct FItemStruct RemoveItem_Implementation(int FromPlace) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Update();
	virtual void Update_Implementation() { return; }
};
