// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Replicator.generated.h"

struct FReplicableProperty
{
public:

	FReplicableProperty(FProperty* OwnerProperty, UStruct* InnerLayout);

	void Replicate(void* ContentPointer, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const;

protected:

	template<typename PropertyType>
	TArray<PropertyType*> FindProperties(UStruct* Layout);

	template<typename PropertyType>
	TArray<FArrayProperty*> FindArrayProperties(UStruct* Layout);

	void ReplicateObjectProperty(void* Owner, const FObjectProperty* ObjectProperty, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const;
	void ReplicateArrayObjectProperty(void* Owner, const FArrayProperty* ArrayObjectProperty, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const;

	void ReplicateObject(class UObject* Object, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const;

	FProperty* OwnerProperty;
	UStruct* InnerLayout;

	TArray<FObjectProperty*> ObjectProperties;
	TArray<FArrayProperty*> ArrayObjectProperties;

	TArray<TUniquePtr<FReplicableProperty>> StructProperties;
	TArray<TUniquePtr<FReplicableProperty>> ArrayStructProperties;
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

	virtual void BuildReplicationGraph();

private:

	TUniquePtr<FReplicableProperty> Graph;

};
