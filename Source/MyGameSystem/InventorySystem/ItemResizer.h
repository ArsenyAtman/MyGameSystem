// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemResizer.generated.h"


class AItem;
class UItemPlace;

UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UItemResizer : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector2D GetInventorySize(const AItem* OwningItem, const UItemPlace* PossessingPlace) const;
	virtual FVector2D GetInventorySize_Implementation(const AItem* OwningItem, const UItemPlace* PossessingPlace) const { return FVector2D::ZeroVector; }

};
