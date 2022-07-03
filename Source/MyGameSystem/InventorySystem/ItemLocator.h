// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemLocator.generated.h"


class AItem;
class UItemPlace;

UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class MYGAMESYSTEM_API UItemLocator : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FTransform GetItemRelativeTransform(UItemPlace* OwningPlace, AItem* Item);
	virtual FTransform GetItemRelativeTransform_Implementation(UItemPlace* OwningPlace, AItem* Item) { return FTransform::Identity; }
};
