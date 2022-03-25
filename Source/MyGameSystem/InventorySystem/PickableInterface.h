// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/InteractionSystem/InteractableActorInterface.h"
#include "PickableInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UPickableInterface : public UInteractableActorInterface
{
	GENERATED_BODY()
};

class MYGAMESYSTEM_API IPickableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetItem(class UItem* Item);
	virtual void SetItem_Implementation(class UItem* Item) = 0;
};
