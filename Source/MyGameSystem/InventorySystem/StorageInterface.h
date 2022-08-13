// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StorageInterface.generated.h"

class AItem;
class UItemPlace;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStorageInterface : public UInterface
{
	GENERATED_BODY()
};

class MYGAMESYSTEM_API IStorageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StorageInterface")
	bool AddItem(AItem* Item);
	virtual bool AddItem_Implementation(AItem* Item) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StorageInterface")
	TArray<AItem*> FindItemsByClass(TSubclassOf<AItem> ItemClass) const;
	virtual TArray<AItem*> FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CheckItemPossession(AItem* Item) const;
	virtual bool CheckItemPossession_Implementation(AItem* Item) const = 0;

};
