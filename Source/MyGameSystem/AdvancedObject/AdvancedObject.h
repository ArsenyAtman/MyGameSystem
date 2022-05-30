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
class MYGAMESYSTEM_API UAdvancedObject : public UReplicableObject
{
	GENERATED_BODY()
	
public:

	// Overrides for the support of the BeginPlay and EventConstruct methods.
	virtual void PostInitProperties() override;

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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (BlueprintProtected))
	void BeginPlay();
	virtual void BeginPlay_Implementation() { return; }

	/**
	 * Called before object destruction.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (BlueprintProtected))
	void EndPlay();
	virtual void EndPlay_Implementation() { return; }

	/**
	 * Called every frame.
	 * @param DeltaTime - A tick delta time in seconds.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (BlueprintProtected))
	void ObjectTick(float DeltaTime);
	virtual void ObjectTick_Implementation(float DeltaTime) { return; }

	/**
	 * Enable/Disable ObjectTick method.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintProtected))
	bool bCanEverTick = true;

private:

	UFUNCTION(NetMulticast, Reliable)
	void MulticastBeginDestroy();

	void SetupTick();
	void RemoveTick();
	
	bool TickerTick(float DeltaTime);

	FTickerDelegate TickDelegate;
	FTSTicker::FDelegateHandle TickDelegateHandle;

};
