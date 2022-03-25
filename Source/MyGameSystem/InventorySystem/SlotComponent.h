// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ChildActorComponent.h"
#include "ItemContainerInterface.h"
#include "SlotComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotUpdateDelegate);

UCLASS(Blueprintable, BlueprintType)
class MYGAMESYSTEM_API USlotComponent : public UChildActorComponent, public IItemContainerInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FSlotUpdateDelegate OnSlotUpdate;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual TArray<struct FItemStruct> GetItems_Implementation() const override;

	virtual bool AddItem_Implementation(struct FItemStruct Item, int InPlace = -1) override;

	virtual struct FItemStruct RemoveItem_Implementation(int FromPlace) override;

	virtual void Update_Implementation() override;

protected:

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void UpdateClient();

private:

	UPROPERTY(ReplicatedUsing = OnRep_PlacedEquipment, EditAnywhere, meta = (AllowPrivateAccess = true))
	struct FItemStruct PlacedEquipment;

	UFUNCTION()
	void OnRep_PlacedEquipment();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UItemType> Type;

	void InitializeItem();
};
