// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RecipeType.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API URecipeType : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	FORCEINLINE FText GetTypeName() { return TypeName; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE FText GetTypeDescription() { return TypeDescription; }

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FText TypeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FText TypeDescription;
};
