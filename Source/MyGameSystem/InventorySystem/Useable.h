// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment.h"
#include "Useable.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UUseable : public UEquipment
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UObject> InfoOfUse;
	
};
