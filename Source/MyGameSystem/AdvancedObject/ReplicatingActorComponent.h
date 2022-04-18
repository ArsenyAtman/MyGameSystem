// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReplicatingActorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UReplicatingActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties.
	UReplicatingActorComponent();

	// Override for the support of ReplicableObjects.
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

protected:

	// Override for the replicator setup.
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	class UReplicator* Replicator = nullptr;

};
