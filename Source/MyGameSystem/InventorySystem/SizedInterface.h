// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Math/Box2D.h"
#include "Math/Vector2D.h"

#include "SizedInterface.generated.h"

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

	virtual FVector2D GetInventoryLocation() const = 0;
	virtual FVector2D GetInventorySize() const = 0;

	virtual FBox2D GetBox() const;
};
