// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SearchLibrary.h"
#include "InventoryComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYGAMESYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInventoryComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	TArray<class APickableInstance*> GetItemsAround(FVector AtLocation);

	UFUNCTION(BlueprintCallable)
	bool DropItem(class UInventoryItem* InventoryItem);

	UFUNCTION(BlueprintCallable)
	TArray<class UInventoryItem*> SearchItemsInInventory(FSearchDelegate Condition);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<class USlot*> GetSlots();
	virtual TArray<class USlot*> GetSlots_Implementation();

	UFUNCTION(BlueprintGetter)
	class USlot* GetVest() { return Vest; }

	UFUNCTION(BlueprintGetter)
	class USlot* GetBelt() { return Belt; }

	UFUNCTION(BlueprintGetter)
	class USlot* GetBackpack() { return Backpack; }

	UFUNCTION(BlueprintGetter)
	class USlot* GetHelmet() { return Helmet; }

	UFUNCTION(BlueprintGetter)
	class USlot* GetWeapon() { return Weapon; }

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, BlueprintGetter = GetVest, Instanced, meta = (AllowPrivateAccess = true))
	class USlot* Vest;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetBelt, Instanced, meta = (AllowPrivateAccess = true))
	class USlot* Belt;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetBackpack, Instanced, meta = (AllowPrivateAccess = true))
	class USlot* Backpack;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetHelmet, Instanced, meta = (AllowPrivateAccess = true))
	class USlot* Helmet;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetWeapon, Instanced, meta = (AllowPrivateAccess = true))
	class USlot* Weapon;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float SphereRadius = 10;
};
