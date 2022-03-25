// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemType.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UItemType : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintGetter)
	FText GetTypeName() const { return TypeName; }

	UFUNCTION(BlueprintGetter)
	FText GetTypeDescription() const  { return TypeDescription; }

private:

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetTypeName, meta = (AllowPrivateAccess = true))
	FText TypeName;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetTypeDescription, meta = (AllowPrivateAccess = true))
	FText TypeDescription;
};
