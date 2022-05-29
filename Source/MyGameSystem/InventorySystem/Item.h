// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "Item.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UItem : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Weight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateBrush IconBrush;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMesh* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTransform MeshTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UItemType> Type;
	
};

USTRUCT(BlueprintType, Blueprintable)
struct FItemStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UObject* OwningContainer; // can't use TScriptInterface, bcz it doesn't support replication

	UPROPERTY(BlueprintReadWrite)
	int Place;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItem* Item;

	bool operator == (const FItemStruct& OtherItem) const;

	FItemStruct(UItem* ItemData = nullptr, UObject* Owner = nullptr, int PositionInContainer = -1)
	{
		OwningContainer = Owner;
		Place = PositionInContainer;
		Item = ItemData;
	}

};