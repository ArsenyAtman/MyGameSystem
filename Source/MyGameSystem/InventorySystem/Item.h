// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InstanceInterface.h"
#include "Item.generated.h"


class UItemPlace;
class UInventoryComponent;

UCLASS()
class MYGAMESYSTEM_API AItem : public AActor, public IInstanceInterface
{
	GENERATED_BODY()

public:

	virtual void Instance_Implementation() override;
	virtual void Uninstance_Implementation() override;

	UFUNCTION(BlueprintGetter)
	UItemPlace* GetPossessingPlace() const { return PossessingPlace; }

	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetRelatedInventory() const;

	UFUNCTION(BlueprintPure)
	bool GetSizeForPlace(TSubclassOf<UItemPlace> PlaceClass, FVector& OutSize);

	UFUNCTION(BlueprintPure)
	bool GetLocationInPlace(FVector& OutLocation);

private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TMap<TSubclassOf<UItemPlace>, FVector> Sizes;
	
	UPROPERTY()
	FVector LocationInPlace;

	UPROPERTY(BlueprintGetter = GetPossessingPlace)
	UItemPlace* PossessingPlace;
	
};
