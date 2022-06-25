// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Replicator.generated.h"

/**
 * Parsing properties with ReplicableObjects of the owner and adding them to the ActorChannel.
 */
UCLASS()
class MYGAMESYSTEM_API UReplicator : public UObject
{

	GENERATED_BODY()

public:

	// Override for the initial object setup.
	virtual void PostInitProperties() override;

	/**
	 * Replicate subobjects of the owner.
	 */
	virtual void ReplicateSubobjectsOfOwner(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);

private:

	virtual void FindPropertiesForReplication(class UClass* Class);

	TArray<class FObjectProperty*> FindObjectPropertiesForReplication(TFieldIterator<class FObjectProperty> Iterator);
	TArray<class FArrayProperty*> FindArrayPropertiesForReplication(TFieldIterator<class FArrayProperty> Iterator);
	
	TArray<class FObjectProperty*> ObjectProperties;
	TArray<class FArrayProperty*> ArrayProperties;

	void ReplicateObjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);
	void ReplicateArrays(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);

	void ReplicateObject(class UObject* Object, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething);

};
