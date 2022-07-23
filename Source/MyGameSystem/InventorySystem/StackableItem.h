// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "StorageInterface.h"

#include "StackableItem.generated.h"


class AItem;

DECLARE_MULTICAST_DYNAMIC_DELEGATE(FCountInStackChangeDelegate, AStackableItem*, StackableItem);

UCLASS()
class MYGAMESYSTEM_API AStackableItem : public AItem, public IStorageInterface
{
	GENERATED_BODY()

public:

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

	UFUNCTION(BlueprintAssignable)
	FCountInStackChangeDelegate OnCountInStackChanged;

protected:

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	int32 MergeWithItem(AStackableItem* Item);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Merged();
	virtual void Merged_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Splitted();
	virtual void Splitted_Implementation() { return; }

private:

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetCountInStack)
	int32 CountInStack;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetMaxCountInStack)
	int32 MaxCountInStack;
	
};
