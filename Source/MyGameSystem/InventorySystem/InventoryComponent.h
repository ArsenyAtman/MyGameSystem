// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SearchLibrary.h"
#include "InventoryComponent.generated.h"


class UPlace;
class UInventoryItem;

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYGAMESYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInventoryComponent();

	UFUNCTION(BlueprintGetter)
	TArray<UPlace*> GetPlaces() const { return Places; }

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, BlueprintGetter = GetPlaces, Instanced, meta = (AllowPrivateAccess = true))
	TArray<UPlace*> Places;
};
