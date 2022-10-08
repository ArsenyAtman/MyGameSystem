// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Replicator.generated.h"

template<typename PropertyType>
struct FPropertyInfo
{
public:

	PropertyType* Property;
	void* Owner;

	FPropertyInfo(PropertyType* PropertyForReplication = nullptr, void* PropertyOwner = nullptr)
	{
		Property = PropertyForReplication;
		Owner = PropertyOwner;
	}
};

struct FReplicableObject
{
	UStruct* StructureLayout;

	TArray<FPropertyInfo<FObjectProperty>> ObjectProperties;
	TArray<FPropertyInfo<FArrayProperty>> ArrayObjectProperties;

	TArray<FPropertyInfo<FObjectProperty>> StructProperties;
	TArray<FPropertyInfo<FArrayProperty>> ArrayStructProperties;

	//template<typename PropertyType>
	//TArray<FPropertyInfo<PropertyType>> FindProperties(void* Owner, UStruct* OwnerLayout);

	//template<typename PropertyType>
	//TArray<FPropertyInfo<FArrayProperty>> FindArrayProperties(void* Owner, UStruct* OwnerLayout);

	//void ReplicateObjectProperty(const FPropertyInfo<FObjectProperty>& PropertyReplicationInfo, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);
	//void ReplicateArrayProperty(const FPropertyInfo<FArrayProperty>& PropertyReplicationInfo, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);

	//void ReplicateObject(class UObject* Object, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);

	FReplicableObject(UStruct* Layout)
	{
		StructureLayout = Layout;
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

	void FindStructProperties(void* Owner, class UStruct* OwnerLayout);

	template<typename PropertyType>
	TArray<FPropertyInfo<PropertyType>> FindProperties(void* Owner, UStruct* OwnerLayout);

	void ReplicateObjectProperty(const FPropertyInfo<FObjectProperty>& PropertyReplicationInfo, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);
	void ReplicateArrayProperty(const FPropertyInfo<FArrayProperty>& PropertyReplicationInfo, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);

	void ReplicateObject(class UObject* Object, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);

private:

	TArray<FPropertyInfo<FObjectProperty>> ObjectProperties;
	TArray<FPropertyInfo<FArrayProperty>> ArrayProperties;

};
