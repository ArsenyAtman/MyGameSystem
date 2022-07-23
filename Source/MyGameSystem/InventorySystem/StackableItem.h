// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "StorageInterface.h"

#include "StackableItem.generated.h"


class AItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCountInStackChangeDelegate, AStackableItem*, StackableItem, int32, NewCountInStack, int32, PrevCountInStack);

UCLASS()
class MYGAMESYSTEM_API AStackableItem : public AItem, public IStorageInterface
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool AddItem_Implementation(AItem* Item) override;
	virtual TArray<AItem*> FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const override { return TArray<AItem*>(); }

	UFUNCTION(BlueprintCallable)
	void InitializeCountInStack(int32 InitCountInStack);

	UFUNCTION(BlueprintGetter)
	int32 GetCountInStack() const { return CountInStack; }

	UFUNCTION(BlueprintGetter)
	int32 GetMaxCountInStack() const { return MaxCountInStack; }

	UFUNCTION(BlueprintCallable)
	void MergedWithItem(int32 CountOfTakenItems);

	UFUNCTION(BlueprintCallable)
	AStackableItem* Split(int32 CountToTake);

	UPROPERTY(BlueprintAssignable)
	FCountInStackChangeDelegate OnCountInStackChanged;

protected:

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	int32 MergeWithItem(AStackableItem* Item);

	UFUNCTION(BlueprintSetter)
	void SetCountInStack(int32 NewCountInStack);

private:

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetCountInStack, BlueprintSetter = SetCountInStack, ReplicatedUsing = OnRep_CountInStack)
	int32 CountInStack;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetMaxCountInStack)
	int32 MaxCountInStack;

	UFUNCTION()
	void OnRep_CountInStack(int32 PrevCountInStack);

	void Broadcast_CountInStackChanged(int32 PrevCountInStack);
	
};
