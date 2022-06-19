// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Instance.generated.h"

UCLASS()
class MYGAMESYSTEM_API AInstance : public AActor
{
	GENERATED_BODY()
	
public:

	AInstance();

	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
#endif

	UFUNCTION(BlueprintGetter)
	class UUniversalMeshComponent* GetUniversalMesh();

	UFUNCTION(BlueprintSetter)
	void SetInventoryItem(class UInventoryItem* NewInventoryItem);

protected:

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Destroyed() override;

	UFUNCTION(BlueprintGetter)
	class UInventoryItem* GetInventoryItem();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class USceneComponent* DefaultRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UUniversalMeshComponent* UniversalMesh;

private:

	UPROPERTY(Instanced, EditAnywhere, BlueprintGetter = GetInventoryItem, BlueprintSetter = SetInventoryItem, meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	class UInventoryItem* InventoryItem;

	class UStaticMesh* DummyMesh;
};
