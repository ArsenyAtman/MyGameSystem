// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicatingActorComponent.h"

#include "Replicator.h"

// Sets default values for this component's properties
UReplicatingActorComponent::UReplicatingActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// This component is replicated by default.
	SetIsReplicatedByDefault(true);

}

void UReplicatingActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// Setup the replicatior.
	Replicator = NewObject<UReplicator>(this);
	Replicator->Initialize(this);

}

bool UReplicatingActorComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	// Delegate all the job to the Replicatior.
	Replicator->ReplicateSubobjectsOfOwner(Channel, Bunch, RepFlags, WroteSomething);

	return WroteSomething;
}