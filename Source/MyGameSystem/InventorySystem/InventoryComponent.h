// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemContainerInterface.h"
#include "Item.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemsUpdateDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotsUpdateDelegate);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYGAMESYSTEM_API UInventoryComponent : public UActorComponent, public IItemContainerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FItemsUpdateDelegate OnItemsUpdate;

	UPROPERTY(BlueprintAssignable)
	FSlotsUpdateDelegate OnSlotsUpdate;

	UPROPERTY(BlueprintAssignable)
	FItemsUpdateDelegate OnContainerOpened;

	UPROPERTY(BlueprintAssignable)
	FItemsUpdateDelegate OnContainerClosed;

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<class USlotComponent*> GetSlots() const { return Slots; }

	UFUNCTION(BlueprintPure)
	TArray<struct FItemStruct> GetItemsOfSlots() const; // was FORCEINLINE

	UFUNCTION(BlueprintPure)
	int GetItemsCountOfClass(class UItem* ItemClass) const;

	UFUNCTION(BlueprintPure)
	int GetItemsCountOfType(TSubclassOf<UItemType> ItemType) const;

	UFUNCTION(BlueprintCallable)
	bool RemoveItemOfClass(class UItem* ItemClass);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemOfType(TSubclassOf<UItemType> ItemType);

	UFUNCTION(BlueprintCallable)
	bool Pickup(class UItem* Item);

	UFUNCTION(BlueprintCallable)
	void Drop(struct FItemStruct Item);

	virtual TArray<struct FItemStruct> GetItems_Implementation() const override { return Items; }

	virtual bool AddItem_Implementation(struct FItemStruct Item, int InPlace = -1) override;

	virtual struct FItemStruct RemoveItem_Implementation(int FromPlace) override;

	virtual void Update_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void OpenContainer(class UObject* Container);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void CloseContainer();

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool HasOpenedContainer() { return IsValid(OpenedContainer); }

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UObject* GetOpenedContainer() { return OpenedContainer; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetInventorySize() { return InventorySize; }

	UFUNCTION(BlueprintPure)
	int GetItemsCount(); // was FORCEINLINE

	// ——— UI interface ———

	// Move an item
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void MoveItem(class UObject* FromContainer, int FromPosition, class UObject* ToContainer, int ToPosition);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void DropItem(class UObject* FromContainer, int FromPosition);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void UpdateClient();

	UPROPERTY(EditDefaultsOnly, BLueprintReadOnly)
	int InventorySize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MustImplement = "PickableInterface"))
	TSubclassOf<AActor> PickableClass;

	UPROPERTY(ReplicatedUsing = OnRep_Items, EditAnywhere, BlueprintReadOnly)
	TArray<struct FItemStruct> Items;

	UPROPERTY(ReplicatedUsing = OnRep_Slots, VisibleAnywhere, BlueprintReadOnly)
	TArray<class USlotComponent*> Slots;

private:

	UFUNCTION()
	void OnRep_Items();

	UFUNCTION()
	void OnRep_Slots();

	TArray<class USlotComponent*> FindAllSlots();

	void InitializeItems();

	UPROPERTY(ReplicatedUsing = OnRep_Container)
	class UObject* OpenedContainer;

	UFUNCTION()
	void OnRep_Container();

	bool CheckContainer(class UObject* Container);
};
