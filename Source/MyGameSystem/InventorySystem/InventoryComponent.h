// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StorageInterface.h"
#include "ComplexStorageInterface.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYGAMESYSTEM_API UInventoryComponent : public UActorComponent, public IStorageInterface, public IComplexStorageInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual bool AddItem_Implementation(AItem* Item);
	virtual TArray<AItem*> FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const;

	virtual TArray<UItemPlace*> GetPlaces_Implementation() const;
		
};
