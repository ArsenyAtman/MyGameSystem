// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Instance.h"
#include "PickableInstance.generated.h"

UCLASS()
class MYGAMESYSTEM_API APickableInstance : public AInstance
{
	GENERATED_BODY()

public:

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UInventoryItem* PickUp();
	virtual UInventoryItem* PickUp_Implementation();

protected:

	virtual void BeginPlay() override;

private:

	FTimerHandle VelocityCheckTimer;

	void VelocityCheck();
};
