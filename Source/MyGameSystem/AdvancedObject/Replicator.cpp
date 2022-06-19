// Fill out your copyright notice in the Description page of Project Settings.


#include "Replicator.h"

#include "Engine/ActorChannel.h"
#include "ReplicableObject.h"
#include "Kismet/KismetSystemLibrary.h"

void UReplicator::PostInitProperties()
{
	Super::PostInitProperties();

	UClass* Class = GetOuter()->GetClass();
	FindPropertiesForReplication(Class);
}

void UReplicator::ReplicateSubobjectsOfOwner(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething)
{
	for (FObjectProperty* ObjectProperty : ObjectProperties)
	{
		// Get the property value.
		UObject* Object = ObjectProperty->GetObjectPropertyValue(ObjectProperty->ContainerPtrToValuePtr<UObject>(GetOuter()));
		// If the property contains a replicable object...
		UReplicableObject* ReplicableObject = Cast<UReplicableObject>(Object);
		if(IsValid(ReplicableObject))
		{
			// than replicate the object ...
			OutWroteSomething |= Channel->ReplicateSubobject(ReplicableObject, *Bunch, *RepFlags);

			// and replicate its subobjects.
			ReplicableObject->ReplicateSubobjects(Channel, Bunch, RepFlags, OutWroteSomething);
		}
	}

	for(FArrayProperty* ArrayProperty : ArrayProperties)
	{
		// Get the property value.
		TArray<UObject*> Array = *ArrayProperty->ContainerPtrToValuePtr<TArray<UObject*>>(GetOuter());
		
		for(UObject* Object : Array)
		{
			// If the array element contains a replicable object...
			UReplicableObject* ReplicableObject = Cast<UReplicableObject>(Object);
			if(IsValid(ReplicableObject))
			{
				// than replicate it ...
				OutWroteSomething |= Channel->ReplicateSubobject(ReplicableObject, *Bunch, *RepFlags);
				// and its subobjects.
				ReplicableObject->ReplicateSubobjects(Channel, Bunch, RepFlags, OutWroteSomething);
			}
		}
	}
}

void UReplicator::FindPropertiesForReplication(UClass* Class)
{
	// Create an iterator for object properties of the class.
	TFieldIterator<FObjectProperty> ObjectsIterator(Class);
	// Find replicable properties.
	ObjectProperties = FindObjectPropertiesForReplication(ObjectsIterator);

	// Create an iterator for array properties of the class.
	TFieldIterator<FArrayProperty> ArraysIterator(Class);
	// Find replicable properties.
	ArrayProperties = FindArrayPropertiesForReplication(ArraysIterator);
}

TArray<FObjectProperty*> UReplicator::FindObjectPropertiesForReplication(TFieldIterator<FObjectProperty> Iterator)
{
	TArray<FObjectProperty*> OutProperties;
	for(; Iterator; ++Iterator)
	{
		// Get the current object property.
		FObjectProperty* ObjectProperty = *Iterator;

		// If the property has the Replicated or RepNotify flags...
		if((ObjectProperty->GetPropertyFlags() & EPropertyFlags::CPF_Net) || (ObjectProperty->GetPropertyFlags() & EPropertyFlags::CPF_RepNotify))
		{
			// than add it to the array.
			OutProperties.Add(ObjectProperty);
		}
	}

	return OutProperties;
}
	
TArray<FArrayProperty*> UReplicator::FindArrayPropertiesForReplication(TFieldIterator<FArrayProperty> Iterator)
{
	TArray<FArrayProperty*> OutProperties;
	for(; Iterator; ++Iterator)
	{
		// Get the current array property.
		FArrayProperty* ArrayProperty = *Iterator;

		// If the property has the Replicated or RepNotify flags...
		if((ArrayProperty->GetPropertyFlags() & EPropertyFlags::CPF_Net) || (ArrayProperty->GetPropertyFlags() & EPropertyFlags::CPF_RepNotify))
		{
			// than add it to the array.
			OutProperties.Add(ArrayProperty);
		}
	}

	return OutProperties;
}