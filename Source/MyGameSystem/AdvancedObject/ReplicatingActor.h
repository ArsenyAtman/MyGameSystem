// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplicatingActor.generated.h"

UCLASS( BlueprintType, Blueprintable )
class MYGAMESYSTEM_API AReplicatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReplicatingActor();

	// Override for the support of ReplicableObjects.
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

private:

	UPROPERTY()
	class UReplicator* Replicator = nullptr;

};
