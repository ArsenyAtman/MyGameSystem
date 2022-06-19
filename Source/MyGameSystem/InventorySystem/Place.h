// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReplicableObject.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItem.h"
#include "Place.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class MYGAMESYSTEM_API UPlace : public UReplicableObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Add(class UInventoryItem* Item, FPoint Location);
	virtual bool Add_Implementation(class UInventoryItem* Item, FPoint Location = FPoint());

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	class UInventoryItem* Remove(class UInventoryItem* Item = nullptr);
	virtual class UInventoryItem* Remove_Implementation(class UInventoryItem* Item = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InstanceItems(class USceneComponent* Mesh);
	virtual void InstanceItems_Implementation(class USceneComponent* Mesh);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DestroyItems();
	virtual void DestroyItems_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<class UInventoryItem*> GetItems();
	virtual TArray<class UInventoryItem*> GetItems_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector TransformLocation(FPoint Location);
	virtual FVector TransformLocation_Implementation(FPoint Location);

	UFUNCTION(BlueprintGetter)
	TArray<TSubclassOf<class UInventoryItem>> GetForSubclassesOf() { return ForSubclassesOf; }

protected:

	UFUNCTION(BlueprintPure)
	bool CheckOnCompability(class UInventoryItem* Item);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Socket = "None";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsInstancing;

	UPROPERTY(BlueprintReadOnly)
	class USceneComponent* LastMesh;

private:

	void SetOwnerToItems(); //Setting Owner for items added manually

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetForSubclassesOf, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class UInventoryItem>> ForSubclassesOf;
	
};
