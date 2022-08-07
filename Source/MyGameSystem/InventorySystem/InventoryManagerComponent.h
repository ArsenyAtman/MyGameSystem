// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"

#include "InventoryManagerComponent.generated.h"


class AItem;
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

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ChangeItemPossession(AItem* Item, FItemPossessionInfo NewPossessionInfo);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void PickupItem(AItem* Item);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void DropItem(AItem* Item);

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

	bool ChangeItemPossession_Validate(AItem* Item, FItemPossessionInfo NewPossessionInfo);
	bool PickupItem_Validate(AItem* Item);
	bool DropItem_Validate(AItem* Item);
		
};
