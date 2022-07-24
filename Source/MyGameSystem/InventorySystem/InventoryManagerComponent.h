// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"

#include "InventoryManagerComponent.generated.h"


class AItem;
class UInventoryComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYGAMESYSTEM_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryManagerComponent();

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

	UFUNCTION(BlueprintPure)
	TArray<UInventoryComponent*> GetConnections() const { return Connections; }

	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetInventoryOfOwner() const;

private:

	TArray<UInventoryComponent*> Connections;

	bool ChangeItemPossession_Validate(AItem* Item, FItemPossessionInfo NewPossessionInfo);
	bool PickupItem_Validate(AItem* Item);
	bool DropItem_Validate(AItem* Item);
		
};
