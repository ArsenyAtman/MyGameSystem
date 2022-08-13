// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StorageInterface.h"
#include "ComplexStorageInterface.h"

#include "InventoryComponent.generated.h"


class AItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemDroppedDelegate, UInventoryComponent*, Inventory, AItem*, Item);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UInventoryComponent : public UActorComponent, public IStorageInterface, public IComplexStorageInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual bool AddItem_Implementation(AItem* Item);
	virtual TArray<AItem*> FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const;
	virtual bool CheckItemPossession_Implementation(AItem* Item) const;

	virtual TArray<UItemPlace*> GetPlaces_Implementation() const;

	UFUNCTION(BlueprintCallable)
	void DropItem(AItem* Item);

	UPROPERTY(BlueprintAssignable)
	FItemDroppedDelegate OnItemDropped;

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ItemDropped(AItem* Item);
	virtual void ItemDropped_Implementation(AItem* Item) { return; }

	UFUNCTION(NetMulticast, Reliable)
	void Notify_ItemDropped(AItem* Item);

	void Broadcast_ItemDropped(AItem* Item);
		
};
