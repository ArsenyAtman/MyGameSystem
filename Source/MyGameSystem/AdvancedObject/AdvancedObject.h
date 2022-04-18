// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReplicableObject.h"
#include "Tickable.h"
#include "AdvancedObject.generated.h"

/**
 * Object with some additional methods:
 * - BeginPlay
 * - EndPlay
 * - ObjectTick
 * - Destroy
 */
UCLASS()
class MYGAMESYSTEM_API UAdvancedObject : public UReplicableObject, public FTickableGameObject
{
	GENERATED_BODY()
	
public:

	// Overrides for the support of the ObjectTick method.
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

	// Overrides for the support of the BeginPlay and EventConstruct methods.
	virtual void PostInitProperties() override;

	// Override for the EndPlay method.
	virtual void BeginDestroy() override;

	// Override for the support of static functions in BP's.
	virtual class UWorld* GetWorld() const override;

	/**
	 * Immediately destroy the object.
	 */
	UFUNCTION(BlueprintCallable)
	void Destroy();

protected:

	/**
	 * Called after object creation.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginPlay();
	virtual void BeginPlay_Implementation() { return; }

	/**
	 * Called before object destruction.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndPlay();
	virtual void EndPlay_Implementation() { return; }

	/**
	 * Called every frame.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ObjectTick(float DeltaTime);
	virtual void ObjectTick_Implementation(float DeltaTime) { return; }

	/**
	 * Enable/Disable the ObjectTick method.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanEverTick = true;

};
