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
	for (const FPropertyInfo<FObjectProperty>& ObjectProperty : ObjectProperties)
	{
		ReplicateObjectProperty(ObjectProperty, Channel, Bunch, RepFlags, OutWroteSomething);
	}

	for (const FPropertyInfo<FArrayProperty>& ArrayProperty : ArrayProperties)
	{
		ReplicateArrayProperty(ArrayProperty, Channel, Bunch, RepFlags, OutWroteSomething);
	}
}

void UReplicator::FindPropertiesForReplication()
{
	void* Owner = GetOuter();
	UStruct* OwnerLayout = GetOuter()->GetClass();

	ObjectProperties = FindProperties<FObjectProperty>(Owner, OwnerLayout);
	ArrayProperties = FindProperties<FArrayProperty>(Owner, OwnerLayout);

	FindStructProperties(Owner, OwnerLayout);
	// FindArraysOfStructs
}

void UReplicator::FindStructProperties(void* Owner, UStruct* OwnerLayout)
{
	TArray<FPropertyInfo<FStructProperty>> Structs = FindProperties<FStructProperty>(Owner, OwnerLayout);
	for (FPropertyInfo<FStructProperty>& Struct : Structs)
	{
		void* StructPointer = Struct.Property->ContainerPtrToValuePtr<void>(Struct.Owner);
		
		ObjectProperties.Append(FindProperties<FObjectProperty>(StructPointer, Struct.Property->Struct));
		ArrayProperties.Append(FindProperties<FArrayProperty>(StructPointer, Struct.Property->Struct));

		FindStructProperties(StructPointer, Struct.Property->Struct);
	}
}

template<typename PropertyType>
TArray<FPropertyInfo<PropertyType>> UReplicator::FindProperties(void* Owner, UStruct* OwnerLayout)
{
	TFieldIterator<PropertyType> Iterator(OwnerLayout);
	TArray<FPropertyInfo<PropertyType>> OutProperties;
	for(; Iterator; ++Iterator)
	{
		// Get the current property.
		PropertyType* Property = *Iterator;

		const bool bCheckReplicationFlags = (Property->GetPropertyFlags() & EPropertyFlags::CPF_Net) || (Property->GetPropertyFlags() & EPropertyFlags::CPF_RepNotify);
		const bool bCheckReplicableTypes = CastField<FObjectProperty>(Property) || CastField<FStructProperty>(Property) || (CastField<FArrayProperty>(Property) && CastField<FObjectProperty>(CastField<FArrayProperty>(Property)->Inner));
		const bool bCheckReplication = (OwnerLayout->IsA(UObject::StaticClass()));
		
		// If the property has the Replicated or RepNotify flags...
		if(bCheckReplicableTypes && (!bCheckReplication || bCheckReplicationFlags))
		{
			// than add it to the array.
			OutProperties.Add(FPropertyInfo<PropertyType>(Property, Owner));
		}
	}

	return OutProperties;
}

void UReplicator::ReplicateObjectProperty(const FPropertyInfo<FObjectProperty>& PropertyReplicationInfo, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething)
{
	// Get the property value.
	UObject* Object = PropertyReplicationInfo.Property->GetObjectPropertyValue(PropertyReplicationInfo.Property->ContainerPtrToValuePtr<UObject>(PropertyReplicationInfo.Owner));

	ReplicateObject(Object, Channel, Bunch, RepFlags, OutWroteSomething);
}

void UReplicator::ReplicateArrayProperty(const FPropertyInfo<FArrayProperty>& PropertyReplicationInfo, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething)
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
