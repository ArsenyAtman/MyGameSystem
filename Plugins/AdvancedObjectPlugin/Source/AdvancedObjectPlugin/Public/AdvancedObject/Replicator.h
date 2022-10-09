// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Replicator.generated.h"

class UActorChannel;
class FOutBunch;
class UObject;

/**
 * Structure unit of a replication graph. Presents an Object or a Struct structure of properties for replication.
 */
struct FObjectForReplication
{
public:

	/**
	 * Recursively build a replication graph for the inner structure of an object or a struct.
	 * @param OwnerProperty - A property that stores a value. Can be nullptr for the root of a replication graph.
	 * @param ObjectLayout - A structure of a value in the OwnerProperty.
	 */
	FObjectForReplication(FProperty* OwnerProperty, UStruct* ObjectLayout);

	/**
	 * Replicate an object instance that matches this replication structure.
	 * @param ObjectPointer - An object instance that can be presented by this replication structure.
	 */
	void Replicate(void* ObjectPointer, UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const;

protected:

	/**
	 * Find replicable properties of a UStruct.
	 */
	template<typename PropertyType>
	TArray<PropertyType*> FindProperties(UStruct* Layout);

	/**
	 * Find replicable array properties of a UStruct.
	 */
	template<typename ElementType>
	TArray<FArrayProperty*> FindArrayProperties(UStruct* Layout);

	/**
	 * Replicate an object property.
	 * @param Object - An instance that owns the ObjectProeprty.
	 * @param ObjectProperty - A property that stores an object for replication.
	 */
	void ReplicateObjectProperty(void* Object, const FObjectProperty* ObjectProperty, UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const;
	
	/**
	 * Replicate an array property of objects.
	 * @param Object - An instance that owns the ObjectProeprty.
	 * @param ObjectProperty - A property that stores an array of objects for replication.
	 */
	void ReplicateArrayObjectProperty(void* Object, const FArrayProperty* ArrayObjectProperty, UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const;

	/**
	 * Replicate an object instance.
	 * @param Object - An object for replication.
	 */
	void ReplicateObject(UObject* Object, UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const;

	/**
	 * A property in the replication tree structure, that stores this object as a property.
	 */
	FProperty* OwnerProperty;

	/**
	 * A structure of this object.
	 */
	UStruct* ObjectLayout;

	/**
	 * Properties with objects for replication.
	 */
	TArray<FObjectProperty*> ObjectProperties;

	/**
	 * Properties with arrays of objects for replication.
	 */
	TArray<FArrayProperty*> ArrayObjectProperties;

	/**
	 * Properties with structures for replication.
	 */
	TArray<TUniquePtr<FObjectForReplication>> StructProperties;

	/**
	 * Properties with arrays of structures for replication.
	 */
	TArray<TUniquePtr<FObjectForReplication>> ArrayStructProperties;
};

/**
 * Managing a replication graph for the owner and handling all replicable subobjects.
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
	virtual void ReplicateSubobjectsOfOwner(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);

protected:

	/**
	 * Build a replication graph for the owner of this component.
	 */
	virtual void BuildReplicationGraph();

private:

	/**
	 * A replication graph.
	 */
	TUniquePtr<FObjectForReplication> ReplicationGraphForOwner;

};
