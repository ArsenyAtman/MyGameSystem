// Fill out your copyright notice in the Description page of Project Settings.


#include "Replicator.h"

#include "Engine/ActorChannel.h"
#include "ReplicableObject.h"
#include "Kismet/KismetSystemLibrary.h"

FObjectForReplication::FObjectForReplication(FProperty* OwnerProperty, UStruct* ObjectLayout)
{
	this->OwnerProperty = OwnerProperty;
	this->ObjectLayout = ObjectLayout;

	ObjectProperties = FindProperties<FObjectProperty>(ObjectLayout);
	ArrayObjectProperties = FindArrayProperties<FObjectProperty>(ObjectLayout);

	TArray<FStructProperty*> FoundStructProperties = FindProperties<FStructProperty>(ObjectLayout);
	for (FStructProperty* FoundStructProperty : FoundStructProperties)
	{
		StructProperties.Add(TUniquePtr<FObjectForReplication>(new FObjectForReplication(FoundStructProperty, FoundStructProperty->Struct)));
	}

	TArray<FArrayProperty*> FoundArrayStructProperties = FindArrayProperties<FStructProperty>(ObjectLayout);
	for (FArrayProperty* FoundArrayStructProperty : FoundArrayStructProperties)
	{
		ArrayStructProperties.Add(TUniquePtr<FObjectForReplication>(new FObjectForReplication(FoundArrayStructProperty, CastField<FStructProperty>(FoundArrayStructProperty->Inner)->Struct)));
	}
}

void FObjectForReplication::Replicate(void* ObjectPointer, UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const
{
	for (FObjectProperty* ObjectProperty : ObjectProperties)
	{
		ReplicateObjectProperty(ObjectPointer, ObjectProperty, Channel, Bunch, RepFlags, OutWroteSomething);
	}

	for (FArrayProperty* ArrayObjectProperty : ArrayObjectProperties)
	{
		ReplicateArrayObjectProperty(ObjectPointer, ArrayObjectProperty, Channel, Bunch, RepFlags, OutWroteSomething);
	}

	for (const TUniquePtr<FObjectForReplication>& StructProperty : StructProperties)
	{
		void* SubobjectPointer = StructProperty->OwnerProperty->ContainerPtrToValuePtr<void*>(ObjectPointer);
		StructProperty->Replicate(SubobjectPointer, Channel, Bunch, RepFlags, OutWroteSomething);
	}

	for (const TUniquePtr<FObjectForReplication>& ArrayStructProperty : ArrayStructProperties)
	{
		FScriptArrayHelper_InContainer ArrayHelper = FScriptArrayHelper_InContainer(CastField<FArrayProperty>(ArrayStructProperty->OwnerProperty), ObjectPointer);
		for (int32 ArrayIndex = 0; ArrayIndex < ArrayHelper.Num(); ++ArrayIndex)
		{
			void* SubobjectPointer = ArrayHelper.GetRawPtr(ArrayIndex);
			ArrayStructProperty->Replicate(SubobjectPointer, Channel, Bunch, RepFlags, OutWroteSomething);
		}
	}
}

template<typename PropertyType>
TArray<PropertyType*> FObjectForReplication::FindProperties(UStruct* Layout)
{
	TFieldIterator<PropertyType> Iterator(Layout);
	TArray<PropertyType*> OutProperties;
	for(; Iterator; ++Iterator)
	{
		PropertyType* Property = *Iterator;

		const bool bCheckReplicationFlags = (Property->GetPropertyFlags() & EPropertyFlags::CPF_Net) || (Property->GetPropertyFlags() & EPropertyFlags::CPF_RepNotify);
		const bool bCheckReplication = (Cast<UClass>(Layout) != nullptr);
		
		if(!bCheckReplication || bCheckReplicationFlags)
		{
			OutProperties.Add(Property);
		}
	}

	return OutProperties;
}

template<typename ElementType>
TArray<FArrayProperty*> FObjectForReplication::FindArrayProperties(UStruct* Layout)
{
	TFieldIterator<FArrayProperty> Iterator(Layout);
	TArray<FArrayProperty*> OutProperties;
	for(; Iterator; ++Iterator)
	{
		FArrayProperty* Property = *Iterator;

		const bool bCheckReplicationFlags = (Property->GetPropertyFlags() & EPropertyFlags::CPF_Net) || (Property->GetPropertyFlags() & EPropertyFlags::CPF_RepNotify);
		const bool bCheckInnerType = (CastField<ElementType>(Property->Inner) != nullptr);
		const bool bCheckReplication = (Cast<UClass>(Layout) != nullptr);
		
		if(bCheckInnerType && (!bCheckReplication || bCheckReplicationFlags))
		{
			OutProperties.Add(Property);
		}
	}

	return OutProperties;
}

void FObjectForReplication::ReplicateObjectProperty(void* Object, const FObjectProperty* ObjectProperty, UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const
{
	UObject* Subobject = *ObjectProperty->ContainerPtrToValuePtr<UObject*>(Object);
	ReplicateObject(Subobject, Channel, Bunch, RepFlags, OutWroteSomething);
}

void FObjectForReplication::ReplicateArrayObjectProperty(void* Object, const FArrayProperty* ArrayObjectProperty, UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const
{
	TArray<UObject*>& ArrayOfSubobjects = *ArrayObjectProperty->ContainerPtrToValuePtr<TArray<UObject*>>(Object);
	for(UObject* Subobject : ArrayOfSubobjects)
	{
		ReplicateObject(Subobject, Channel, Bunch, RepFlags, OutWroteSomething);
	}
}

void FObjectForReplication::ReplicateObject(UObject* Object, UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const
{
	UReplicableObject* ReplicableObject = Cast<UReplicableObject>(Object);
	if(IsValid(ReplicableObject) && !ReplicableObject->GetIsReplicatingNow())
	{
		OutWroteSomething |= Channel->ReplicateSubobject(ReplicableObject, *Bunch, *RepFlags);
		ReplicableObject->ReplicateSubobjects(Channel, Bunch, RepFlags, OutWroteSomething);
	}
}

void UReplicator::PostInitProperties()
{
	Super::PostInitProperties();

	BuildReplicationGraph();
}

void UReplicator::ReplicateSubobjectsOfOwner(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething)
{
	UObject* Owner = this->GetOuter();
	ReplicationGraphForOwner->Replicate(Owner, Channel, Bunch, RepFlags, OutWroteSomething);
}

void UReplicator::BuildReplicationGraph()
{
	UClass* OwnerClass = this->GetOuter()->GetClass();
	ReplicationGraphForOwner = TUniquePtr<FObjectForReplication>(new FObjectForReplication(nullptr, OwnerClass));
}
