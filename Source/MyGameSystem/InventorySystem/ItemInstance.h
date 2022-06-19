// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInstance.generated.h"


class UItem;

UCLASS()
class MYGAMESYSTEM_API AItemInstance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemInstance();

	UFUNCTION(BlueprintGetter)
	UItem* GetRelatedItem() const { return RelatedItem; }

	virtual void Initialize(UItem* InitialRelatedItem) { RelatedItem = InitialRelatedItem; }

private:
	
	UPROPERTY(Instanced, EditAnywhere, BlueprintGetter = GetRelatedItem, meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	UItem* RelatedItem;

};
