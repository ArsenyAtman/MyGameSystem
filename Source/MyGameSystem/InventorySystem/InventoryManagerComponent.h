// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"

#include "InventoryManagerComponent.generated.h"


class AItem;
class AComplexItem;
class AStackableItem;
class UInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryManagerConnectionDelegate, UInventoryManagerComponent*, InventoryManager, UInventoryComponent*, InventoryComponent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYGAMESYSTEM_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryManagerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ChangeItemPossession(AItem* Item, FItemPossessionInfo NewPossessionInfo);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void PickupItem(AItem* Item);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void DropItem(AItem* Item);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AddItemToComplexItem(AItem* Item, AComplexItem* ComplexItem);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void MergeStackableItems(AStackableItem* ReceivingItem, AStackableItem* IncomingItem);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SplitStackableItem(AStackableItem* ItemToSplit, int32 CountToTake, FItemPossessionInfo PossessionInfo);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SplitAndMergeStackableItem(AStackableItem* ReceivingItem, AStackableItem* ItemToSplit, int32 CountToTake);

	UFUNCTION(BlueprintCallable)
	bool Connect(UInventoryComponent* InventoryComponent);

	UFUNCTION(BlueprintCallable)
	bool Disconnect(UInventoryComponent* InventoryComponent);

	UFUNCTION(BlueprintGetter)
	TArray<UInventoryComponent*> GetConnections() const { return Connections; }

	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetInventoryOfOwner() const;

	UPROPERTY(BlueprintAssignable)
	FInventoryManagerConnectionDelegate OnInventoryConnected;

	UPROPERTY(BlueprintAssignable)
	FInventoryManagerConnectionDelegate OnInventoryDisconnected;

protected:

	// ...

private:

	UPROPERTY(BlueprintGetter = GetConnections, ReplicatedUsing = OnRep_Connections, meta = (AllowPrivateAccess))
	TArray<UInventoryComponent*> Connections;

	UFUNCTION()
	void OnRep_Connections(const TArray<UInventoryComponent*>& PrevConnections);
		
};
