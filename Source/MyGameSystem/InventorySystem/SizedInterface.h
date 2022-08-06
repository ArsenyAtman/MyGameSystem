// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Math/Box2D.h"
#include "Math/Vector2D.h"

#include "SizedInterface.generated.h"


class UItemPlace;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USizedInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYGAMESYSTEM_API ISizedInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SizedInterface")
	FVector2D GetInventoryLocation() const;
	virtual FVector2D GetInventoryLocation_Implementation() const { return FVector2D::ZeroVector; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SizedInterface")
	FVector2D GetInventorySize() const;
	virtual FVector2D GetInventorySize_Implementation() const = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SizedInterface")
	FVector2D GetInventorySizeForPlace(UItemPlace* Place) const;
	virtual FVector2D GetInventorySizeForPlace_Implementation(UItemPlace* Place) const { return FVector2D::ZeroVector; }

	virtual FBox2D GetBox() const;
	virtual FBox2D GetBoxForPlace(FVector2D Location, UItemPlace* Place) const;
};
