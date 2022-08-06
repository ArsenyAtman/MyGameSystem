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

USTRUCT(BlueprintType, Blueprintable)
struct MYGAMESYSTEM_API FItemPossessionInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D InventoryLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UItemPlace* PossessingPlace;

	FItemPossessionInfo(FVector2D Location = FVector2D::ZeroVector, UItemPlace* Place = nullptr)
	{
		InventoryLocation = Location;
		PossessingPlace = Place;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FItemPossessionChangeDelegate, AItem*, Item, FItemPossessionInfo, NewPossessionInfo, FItemPossessionInfo, PrevPossessionInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemConditionChangeDelegate, AItem*, Item);

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API AItem : public AActor, public IInstanceInterface, public ISizedInterface
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Instance_Implementation() override;
	virtual void Uninstance_Implementation() override;

	virtual FVector2D GetInventoryLocation_Implementation() const override { return ItemPossession.InventoryLocation; }
	virtual FVector2D GetInventorySize_Implementation() const override;
	virtual FVector2D GetInventorySizeForPlace_Implementation(UItemPlace* Place) const override;

	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetRelatedInventory() const;

	UFUNCTION(BlueprintPure)
	UItemPlace* GetPossessingPlace() const { return ItemPossession.PossessingPlace; }

	UFUNCTION(BlueprintCallable)
	void PlaceInPlace(UItemPlace* NewPlace, FVector2D NewLocation);

	UFUNCTION(BlueprintCallable)
	void RemoveFromPlace();

	UPROPERTY(BlueprintAssignable)
	FItemPossessionChangeDelegate OnPossessionChanged;

	UPROPERTY(BlueprintAssignable)
	FItemConditionChangeDelegate OnResized;

	UPROPERTY(BlueprintAssignable)
	FItemConditionChangeDelegate OnInstanced;

	UPROPERTY(BlueprintAssignable)
	FItemConditionChangeDelegate OnUninstanced;

protected:

	UPROPERTY(Instanced, EditDefaultsOnly, BlueprintReadOnly)
	UItemResizer* ItemResizer;

	UFUNCTION(BlueprintCallable)
	void SetPossession(FItemPossessionInfo NewPossessionInfo);

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

	UPROPERTY(BlueprintSetter = SetPossession, ReplicatedUsing = OnRep_ItemPossession)
	FItemPossessionInfo ItemPossession;

	UFUNCTION()
	void OnRep_ItemPossession(FItemPossessionInfo PrevPossession);

	void Broadcast_PossessionChanged(FItemPossessionInfo PrevPossession);

	void Broadcast_ItemInstanced();
	void Broadcast_ItemUninstanced();

	UFUNCTION(NetMulticast, Reliable)
	void InstanceItem();

	UFUNCTION(NetMulticast, Reliable)
	void UninstanceItem();
};
