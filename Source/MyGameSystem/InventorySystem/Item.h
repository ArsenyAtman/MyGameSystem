// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InstanceInterface.h"
#include "SizedInterface.h"

#include "Item.generated.h"


class AItem;
class UItemPlace;
class UInventoryComponent;
class UItemResizer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemPossessionChangeDelegate, AItem*, Item, UItemPlace*, ItemPlace);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemConditionChangeDelegate, AItem*, Item);

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API AItem : public AActor, public IInstanceInterface, public ISizedInterface
{
	GENERATED_BODY()

public:

	virtual void Instance_Implementation() override;
	virtual void Uninstance_Implementation() override;

	virtual FVector2D GetInventoryLocation() const override { return InventoryLocation; }
	virtual FVector2D GetInventorySize() const override;

	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetRelatedInventory() const;

	UFUNCTION(BlueprintGetter)
	UItemPlace* GetPossessingPlace() const { return PossessingPlace; }

	UFUNCTION(BlueprintCallable)
	void PlaceInPlace(UItemPlace* NewPlace, FVector2D NewLocation);

	UFUNCTION(BlueprintCallable)
	void RemoveFromPlace();

	UPROPERTY(BlueprintAssignable)
	FItemPossessionChangeDelegate OnPlaced;

	UPROPERTY(BlueprintAssignable)
	FItemPossessionChangeDelegate OnRemoved;

	UPROPERTY(BlueprintAssignable)
	FItemConditionChangeDelegate OnDropped;

	UPROPERTY(BlueprintAssignable)
	FItemConditionChangeDelegate OnResized;

	UPROPERTY(BlueprintAssignable)
	FItemConditionChangeDelegate OnInstanced;

	UPROPERTY(BlueprintAssignable)
	FItemConditionChangeDelegate OnUninstanced;

protected:

	UPROPERTY(Instanced, EditDefaultsOnly, BlueprintReadOnly)
	UItemResizer* ItemResizer;

	UFUNCTION(BlueprintSetter)
	void SetInventoryLocation(FVector2D NewLocation) { InventoryLocation = NewLocation; }

	UFUNCTION(BlueprintSetter)
	void SetPossessingPlace(UItemPlace* NewPossessingPlace) { PossessingPlace = NewPossessingPlace; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Placed(UItemPlace* ItemPlace);
	virtual void Placed_Implementation(UItemPlace* ItemPlace) { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Removed(UItemPlace* ItemPlace);
	virtual void Removed_Implementation(UItemPlace* ItemPlace) { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ShowView();
	virtual void ShowView_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HideView();
	virtual void HideView_Implementation() { return; }

private:

	UPROPERTY(BlueprintSetter = SetInventoryLocation)
	FVector2D InventoryLocation;

	UPROPERTY(BlueprintGetter = GetPossessingPlace, BlueprintSetter = SetPossessingPlace)
	UItemPlace* PossessingPlace;
};
