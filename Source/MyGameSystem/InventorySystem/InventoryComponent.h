// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/ReplicatingActorComponent.h"
#include "StorageInterface.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYGAMESYSTEM_API UInventoryComponent : public UReplicatingActorComponent, public IStorageInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual TArray<UItemPlace*> GetItemPlaces_Implementation() const override { return TArray<UItemPlace*>(); }
		
};
