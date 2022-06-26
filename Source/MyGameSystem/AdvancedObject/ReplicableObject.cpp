// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicableObject.h"

#include "ReplicatingActorComponent.h"
#include "Replicator.h"
#include "Kismet/KismetSystemLibrary.h"

UReplicableObject::UReplicableObject()
{
    // Create a replicator subobject.
    Replicator = CreateDefaultSubobject<UReplicator>(TEXT("Replicator"), false);
}

void UReplicableObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Add BP properties to replication.
    UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass());
    if (BPClass)
    {
        BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
    }
}

void UReplicableObject::PostInitProperties()
{
    Super::PostInitProperties();

    // Find and set the first outer actor for this object.
    FirstOuterActor = FindFirstOuterActor();
    
}

void UReplicableObject::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething)
{
    // This object is replicating now (cyclic pointers protection).
    SetIsReplicatingNow(true);

    // Delegate all the job to the Replicatior.
    Replicator->ReplicateSubobjectsOfOwner(Channel, Bunch, RepFlags, OutWroteSomething);

    // This object is not replicating now.
    SetIsReplicatingNow(false);
}

bool UReplicableObject::CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParameters, FFrame* Stack)
{
    AActor* OuterActor = GetFirstOuterActor();
    if (IsValid(OuterActor))
    {
        UNetDriver* NetDriver = OuterActor->GetNetDriver();
        if (IsValid(NetDriver))
        {
            NetDriver->ProcessRemoteFunction(OuterActor, Function, Parameters, OutParameters, Stack, this);
            return true;
        }
    }
    return false;
}

int32 UReplicableObject::GetFunctionCallspace(UFunction * Function, FFrame * Stack)
{
    AActor* OuterActor = GetFirstOuterActor();
    if (IsValid(OuterActor))
    {
        return OuterActor->GetFunctionCallspace(Function, Stack);
    }
 
    return FunctionCallspace::Local;
}

bool UReplicableObject::HasAuthority() const
{
    return GetNetRole() == ENetRole::ROLE_Authority;
}

ENetRole UReplicableObject::GetNetRole() const
{
    if(IsValid(GetFirstOuterActor()))
    {
        return GetFirstOuterActor()->GetLocalRole();
    }

    return ENetRole::ROLE_None;
}

AActor* UReplicableObject::FindFirstOuterActor()
{
    UObject* Outer = GetOuter();
    while(IsValid(Outer))
    {
        // If the next outer is ActorComponent...
        UActorComponent* ActorComponent = Cast<UActorComponent>(Outer);
        if(IsValid(ActorComponent))
        {
            // than return its owner.
            return ActorComponent->GetOwner();
        }
        
        // If the next outer is Actor...
        AActor* Actor = Cast<AActor>(Outer);
        if (IsValid(Actor))
        {
            // than return it.
            return Actor;
        }

        // Go to the next outer.
        Outer = Outer->GetOuter();
    }

    return nullptr;
}
