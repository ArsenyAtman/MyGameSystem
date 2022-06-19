// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSYstem/AdvancedObject/AdvancedObject.h"
#include "InstancingInterface.h"
#include "ItemPlace.generated.h"


class UItem;

UCLASS(Abstract)
class MYGAMESYSTEM_API UItemPlace : public UAdvancedObject, public IInstancingInterface
{
	GENERATED_BODY()

public:

	virtual void Instance_Implementation(AItemInstance* ParentInstance) override;
	virtual void Uninstance_Implementation() override;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	TArray<UItem*> GetItems() const;
	virtual TArray<UItem*> GetItems_Implementation() const { return TArray<UItem*>(); }
	
};
