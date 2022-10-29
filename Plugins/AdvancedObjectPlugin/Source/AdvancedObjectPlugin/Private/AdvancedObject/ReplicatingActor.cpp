// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedObject/ReplicatingActor.h"

#include "AdvancedObject/Replicator.h"

// Sets default values
AReplicatingActor::AReplicatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// This actor is replicated by default.
	bReplicates = true;

	// Create a replicator subobject.
	Replicator = CreateDefaultSubobject<UReplicator>(TEXT("Replicator"), false);

}

bool AReplicatingActor::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	// Delegate all the job to the Replicatior.
	Replicator->ReplicateSubobjectsOfOwner(Channel, Bunch, RepFlags, WroteSomething);

	return WroteSomething;
}
