// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReplicableObject.generated.h"

/**
 * Object with support of replication and RPC's.
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UReplicableObject : public UObject
{
	GENERATED_BODY()

public:

	// Override for the initial setup of this object.
	virtual void PostInitProperties() override;

	// Overrides for the support of replicable propeerties.
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Overrides for the support of RPC's
	virtual bool CallRemoteFunction(UFunction* Function, void* Parameters, struct FOutParmRec* OutParameters, FFrame* Stack) override;
 	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;

	/**
	 * Allows to replicate sub-ReplicableObjects that have been marked for replication.
	 */
	virtual void ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);

protected:

	/**
	 * The first outer actor that was found by following the chain of outers for this object.
	 */
	UFUNCTION(BlueprintGetter)
	class AActor* GetFirstOuterActor() const { return FirstOuterActor; }

private:

	UPROPERTY()
	class UReplicator* Replicator = nullptr;

	UPROPERTY(BlueprintGetter = GetFirstOuterActor)
	class AActor* FirstOuterActor = nullptr;

	class AActor* FindFirstOuterActor();

};
