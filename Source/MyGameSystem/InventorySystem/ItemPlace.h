// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "StorageInterface.h"
#include "InstanceInterface.h"
#include "SizedInterface.h"

#include "ItemPlace.generated.h"


class AItem;
class UObject;
class UItemPlace;
class UItemLocator;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemPlaceConditionDelegate, UItemPlace*, ItemPlace, AItem*, Item);

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UItemPlace : public USceneComponent, public IStorageInterface, public IInstanceInterface, public ISizedInterface
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	TArray<AItem*> GetItems() const;
	virtual TArray<AItem*> GetItems_Implementation() const { return TArray<AItem*>(); }

	virtual bool AddItem_Implementation(AItem* Item);
	virtual TArray<AItem*> FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const;

	virtual void Instance_Implementation() override;
	virtual void Uninstance_Implementation() override;

	virtual FVector2D GetInventoryLocation() const override { return FVector2D::ZeroVector; }
	virtual FVector2D GetInventorySize() const override { return PlaceSize; }

	UFUNCTION(BlueprintGetter)
	UObject* GetPossessor() const;

	UFUNCTION(BlueprintCallable)
	bool PlaceItem(AItem* Item, FVector2D Position);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(AItem* Item);

	UFUNCTION(BlueprintGetter)
	bool GetIsInstancing() { return bIsInstancing; }

	UFUNCTION(BlueprintSetter)
	void SetIsInstancing(bool bNewIsInstancing);

	UFUNCTION(BlueprintPure)
	FTransform GetRelativeTransformForItem(AItem* Item);

	UPROPERTY(BlueprintAssignable)
	FItemPlaceConditionDelegate OnPlaced;

	UPROPERTY(BlueprintAssignable)
	FItemPlaceConditionDelegate OnRemoved;

protected:

	UPROPERTY(Instanced, EditDefaultsOnly, BlueprintReadOnly)
	UItemLocator* ItemLocator;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ItemPlaced(AItem* Item);
	virtual void ItemPlaced_Implementation(AItem* Item) { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ItemRemoved(AItem* Item);
	virtual void ItemRemoved_Implementation(AItem* Item) { return; }

private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FIntPoint PlaceSize;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float PlaceGridSize;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FRotator ItemsRelativeRotation;

	UPROPERTY(ReplicatedUsing = OnRep_Items)
	TArray<AItem*> Items;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetIsInstancing, BlueprintSetter = SetIsInstancing, meta = (AllowPrivateAccess = true))
	bool bIsInstancing = true;

	UFUNCTION()
	void OnRep_Items(const TArray<AItem*>& PrevItems);

	void BroadcastChange_Items(const TArray<AItem*>& PrevItems);
	void Broadcast_ItemPlaced(AItem* Item);
	void Broadcast_ItemRemoved(AItem* Item);
	
};
