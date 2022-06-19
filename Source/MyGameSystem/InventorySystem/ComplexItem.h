// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Place.h"
#include "ComplexItem.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UComplexItem : public UInventoryItem
{
	GENERATED_BODY()
	
public:

	virtual void InitializeInActor_Implementation(class AInstance* InstanceActor) override;

	virtual void DestroySubinstances_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InstancePlaces(USceneComponent* Mesh);
	virtual void InstancePlaces_Implementation(USceneComponent* Mesh);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<UPlace*> GetPlaces();
	virtual TArray<UPlace*> GetPlaces_Implementation();

private:

	UPROPERTY(EditAnywhere, Instanced, meta = (AllowPrivateAccess = true))
	TArray<UPlace*> Places;
};
