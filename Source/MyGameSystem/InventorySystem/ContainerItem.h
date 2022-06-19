// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "ContainerItem.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UContainerItem : public UInventoryItem
{
	GENERATED_BODY()

public:

	UContainerItem();

	virtual void InitializeInActor_Implementation(class AInstance* InstanceActor) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Add(class UInventoryItem* Item);
	virtual void Add_Implementation(class UInventoryItem* Item);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UInventoryItem* Pop();
	virtual class UInventoryItem* Pop_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	class UInventoryItem* GetLastItem();
	virtual class UInventoryItem* GetLastItem_Implementation();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	class UContainer* Container;
};
