// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "StorageInterface.h"
#include "InstanceInterface.h"
#include "ItemPlace.generated.h"


class UItem;
class UObject;

UCLASS(Abstract)
class MYGAMESYSTEM_API UItemPlace : public USceneComponent, public IStorageInterface, public IInstanceInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	TArray<AItem*> GetItems() const;
	virtual TArray<AItem*> GetItems_Implementation() const { return TArray<AItem*>(); }

	virtual bool AddItem_Implementation(AItem* Item);
	virtual TArray<AItem*> FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const;

	virtual void Instance_Implementation() override;
	virtual void Uninstance_Implementation() override;

	UFUNCTION(BlueprintGetter)
	UObject* GetPossessor() const;
	
};
