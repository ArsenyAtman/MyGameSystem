// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Instance.generated.h"


class UInventoryItem;

UCLASS()
class MYGAMESYSTEM_API AInstance : public AActor
{
	GENERATED_BODY()
	
public:

	AInstance();

	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
#endif

	UFUNCTION(BlueprintSetter)
	void SetInventoryItem(UInventoryItem* NewInventoryItem) { InventoryItem = NewInventoryItem; }

protected:

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Destroyed() override;

	UFUNCTION(BlueprintGetter)
	UInventoryItem* GetInventoryItem() const { return InventoryItem; }

private:

	UPROPERTY(Instanced, EditAnywhere, BlueprintGetter = GetInventoryItem, BlueprintSetter = SetInventoryItem, meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	UInventoryItem* InventoryItem;

};
