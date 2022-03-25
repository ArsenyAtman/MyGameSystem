// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RecipeDataAsset.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FCraftItemStruct
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UItem* Item;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Count;

	FCraftItemStruct(class UItem* ItemForCraft = nullptr, int CountOfItems = 1)
	{
		Item = ItemForCraft;
		Count = CountOfItems;
	}
};

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API URecipeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText RecipeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText RecipeDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class URecipeType> RecipeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCraftItemStruct> RequiredItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCraftItemStruct> CraftedItems;
	
};
