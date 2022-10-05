// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Replicator.generated.h"

template<typename PropertyType>
struct FPropertyReplicationInfo
{
public:

	PropertyType* Property;
	void* Owner;

	FPropertyReplicationInfo(PropertyType* PropertyForReplication = nullptr, void* PropertyOwner = nullptr)
	{
		Property = PropertyForReplication;
		Owner = PropertyOwner;
	}
};

/**
 * Parsing properties with ReplicableObjects of the owner and adding them to the ActorChannel.
 */
UCLASS()
class ADVANCEDOBJECTPLUGIN_API UReplicator : public UObject
{

	GENERATED_BODY()

public:

	// Override for the initial object setup.
	virtual void PostInitProperties() override;

	/**
	 * Replicate subobjects of the owner.
	 */
	virtual void ReplicateSubobjectsOfOwner(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);

protected:

	virtual void FindPropertiesForReplication();

	template<typename PropertyType>
	TArray<FPropertyReplicationInfo<PropertyType>> FindPropertiesOfObjectForReplication(bool bCheckOnReplication, void* Owner, UStruct* OwnerLayout);

	void ReplicateObjectProperty(const FPropertyReplicationInfo<FObjectProperty>& PropertyReplicationInfo, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);
	void ReplicateArrayProperty(const FPropertyReplicationInfo<FArrayProperty>& PropertyReplicationInfo, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);

	void ReplicateObject(class UObject* Object, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);

private:

	TArray<FPropertyReplicationInfo<FObjectProperty>> ObjectPropertiesForReplication;
	TArray<FPropertyReplicationInfo<FArrayProperty>> ArrayPropertiesForReplication;

};
