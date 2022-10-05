// Fill out your copyright notice in the Description page of Project Settings.


#include "Replicator.h"

#include "Engine/ActorChannel.h"
#include "ReplicableObject.h"
#include "Kismet/KismetSystemLibrary.h"

void UReplicator::PostInitProperties()
{
	Super::PostInitProperties();

	FindPropertiesForReplication();
}

void UReplicator::ReplicateSubobjectsOfOwner(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething)
{
	for (const FPropertyReplicationInfo<FObjectProperty>& ObjectPropertyForReplication : ObjectPropertiesForReplication)
	{
		ReplicateObjectProperty(ObjectPropertyForReplication, Channel, Bunch, RepFlags, OutWroteSomething);
	}

	for (const FPropertyReplicationInfo<FArrayProperty>& ArrayPropertyForReplication : ArrayPropertiesForReplication)
	{
		ReplicateArrayProperty(ArrayPropertyForReplication, Channel, Bunch, RepFlags, OutWroteSomething);
	}
}

void UReplicator::FindPropertiesForReplication()
{
	void* Owner = GetOuter();
	UStruct* OwnerLayout = GetOuter()->GetClass();

	ObjectPropertiesForReplication = FindPropertiesOfObjectForReplication<FObjectProperty>(true, Owner, OwnerLayout);
	ArrayPropertiesForReplication = FindPropertiesOfObjectForReplication<FArrayProperty>(true, Owner, OwnerLayout);

	TArray<FPropertyReplicationInfo<FStructProperty>> StructsForReplication = FindPropertiesOfObjectForReplication<FStructProperty>(true, Owner, OwnerLayout);
	for (FPropertyReplicationInfo<FStructProperty>& StructForReplication : StructsForReplication)
	{
		void* StructPointer = StructForReplication.Property->ContainerPtrToValuePtr<void>(StructForReplication.Owner);
		
		ObjectPropertiesForReplication.Append(FindPropertiesOfObjectForReplication<FObjectProperty>(false, StructPointer, StructForReplication.Property->Struct));
		ArrayPropertiesForReplication.Append(FindPropertiesOfObjectForReplication<FArrayProperty>(false, StructPointer, StructForReplication.Property->Struct));
	}
}

template<typename PropertyType>
TArray<FPropertyReplicationInfo<PropertyType>> UReplicator::FindPropertiesOfObjectForReplication(bool bCheckOnReplication, void* Owner, UStruct* OwnerLayout)
{
	TFieldIterator<PropertyType> Iterator(OwnerLayout);
	TArray<FPropertyReplicationInfo<PropertyType>> OutPropertiesForReplication;
	for(; Iterator; ++Iterator)
	{
		// Get the current property.
		PropertyType* Property = *Iterator;

		// If the property has the Replicated or RepNotify flags...
		if(!bCheckOnReplication || ((Property->GetPropertyFlags() & EPropertyFlags::CPF_Net) || (Property->GetPropertyFlags() & EPropertyFlags::CPF_RepNotify)))
		{
			// than add it to the array.
			OutPropertiesForReplication.Add(FPropertyReplicationInfo<PropertyType>(Property, Owner));
		}
	}

	return OutPropertiesForReplication;
}

void UReplicator::ReplicateObjectProperty(const FPropertyReplicationInfo<FObjectProperty>& PropertyReplicationInfo, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething)
{
	// Get the property value.
	UObject* Object = PropertyReplicationInfo.Property->GetObjectPropertyValue(PropertyReplicationInfo.Property->ContainerPtrToValuePtr<UObject>(PropertyReplicationInfo.Owner));

	ReplicateObject(Object, Channel, Bunch, RepFlags, OutWroteSomething);
}

void UReplicator::ReplicateArrayProperty(const FPropertyReplicationInfo<FArrayProperty>& PropertyReplicationInfo, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething)
{
	// Get the property value.
	TArray<UObject*> Array = *(PropertyReplicationInfo.Property)->ContainerPtrToValuePtr<TArray<UObject*>>(PropertyReplicationInfo.Owner);

	for(UObject* Object : Array)
	{
		ReplicateObject(Object, Channel, Bunch, RepFlags, OutWroteSomething);
	}
}

void UReplicator::ReplicateObject(UObject* Object, UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething)
{
	// If the object is replicable and is not being replicated already...
	UReplicableObject* ReplicableObject = Cast<UReplicableObject>(Object);
	if(IsValid(ReplicableObject) && !ReplicableObject->GetIsReplicatingNow())
	{
		// than replicate it...
		OutWroteSomething |= Channel->ReplicateSubobject(ReplicableObject, *Bunch, *RepFlags);
		// and its subobjects.
		ReplicableObject->ReplicateSubobjects(Channel, Bunch, RepFlags, OutWroteSomething);
	}
}
