

#pragma once

#include "CoreMinimal.h"
#include "InventorySystemTypes.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int X = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Y = 0;

	FPoint()
	{
		X = 0;
		Y = 0;
	}

	FPoint(int x, int y)
	{
		X = x;
		Y = y;
	}

	FPoint operator + (FPoint Point)
	{
		return FPoint(this->X + Point.X, this->Y + Point.Y);
	}

	FPoint operator - (FPoint Point)
	{
		return FPoint(this->X - Point.X, this->Y - Point.Y);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FSocketInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* Component;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTransform Transform;

	FSocketInfo()
	{
		Component = nullptr;
		Name = "None";
		Transform = FTransform::Identity;
	}
};