// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "InstancingInterface.h"
#include "Item.generated.h"


class AItemInstance;

UCLASS()
class MYGAMESYSTEM_API UItem : public UAdvancedObject, public IInstancingInterface
{
	GENERATED_BODY()

public:

	virtual void Instance_Implementation(AItemInstance* ParentInstance) override;
	virtual void Uninstance_Implementation() override;
	
	UFUNCTION(BlueprintGetter)
	AItemInstance* GetItemInstance() const { return ItemInstance; }

private:

	void SetItemInstance(AItemInstance* NewItemInstance);

	UPROPERTY(BlueprintGetter = GetItemInstance)
	AItemInstance* ItemInstance;
	
};
