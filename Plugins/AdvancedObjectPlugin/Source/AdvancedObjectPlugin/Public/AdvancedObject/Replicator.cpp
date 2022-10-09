// Fill out your copyright notice in the Description page of Project Settings.


#include "Replicator.h"

#include "Engine/ActorChannel.h"
#include "ReplicableObject.h"
#include "Kismet/KismetSystemLibrary.h"

FReplicableProperty::FReplicableProperty(FProperty* OwnerProperty, UStruct* InnerLayout)
{
	this->OwnerProperty = OwnerProperty;
	this->InnerLayout = InnerLayout;

	ObjectProperties = FindProperties<FObjectProperty>(InnerLayout);
	ArrayObjectProperties = FindArrayProperties<FObjectProperty>(InnerLayout);

	TArray<FStructProperty*> FoundStructProperties = FindProperties<FStructProperty>(InnerLayout);
	for (FStructProperty* FoundStructProperty : FoundStructProperties)
	{
		StructProperties.Add(TUniquePtr<FReplicableProperty>(new FReplicableProperty(FoundStructProperty, FoundStructProperty->Struct)));
	}

	TArray<FArrayProperty*> FoundArrayStructProperties = FindArrayProperties<FStructProperty>(InnerLayout);
	for (FArrayProperty* FoundArrayStructProperty : FoundArrayStructProperties)
	{
		ArrayStructProperties.Add(TUniquePtr<FReplicableProperty>(new FReplicableProperty(FoundArrayStructProperty, CastField<FStructProperty>(FoundArrayStructProperty->Inner)->Struct)));
	}
}

void FReplicableProperty::Replicate(void* OwnerPointer, class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const
{
	for (FObjectProperty* ObjectProperty : ObjectProperties)
	{
		ReplicateObjectProperty(OwnerPointer, ObjectProperty, Channel, Bunch, RepFlags, OutWroteSomething);
	}

	for (FArrayProperty* ArrayObjectProperty : ArrayObjectProperties)
	{
		ReplicateArrayObjectProperty(OwnerPointer, ArrayObjectProperty, Channel, Bunch, RepFlags, OutWroteSomething);
	}

	for (const TUniquePtr<FReplicableProperty>& StructProperty : StructProperties)
	{
		void* InnerPointer = StructProperty->OwnerProperty->ContainerPtrToValuePtr<void*>(OwnerPointer);
		StructProperty->Replicate(InnerPointer, Channel, Bunch, RepFlags, OutWroteSomething);
	}

	for (const TUniquePtr<FReplicableProperty>& ArrayStructProperty : ArrayStructProperties)
	{
		FScriptArrayHelper_InContainer ArrayHelper = FScriptArrayHelper_InContainer(CastField<FArrayProperty>(ArrayStructProperty->OwnerProperty), OwnerPointer);
		for (int32 ArrayIndex = 0; ArrayIndex < ArrayHelper.Num(); ++ArrayIndex)
		{
			void* InnerPointer = ArrayHelper.GetRawPtr(ArrayIndex);
			ArrayStructProperty->Replicate(InnerPointer, Channel, Bunch, RepFlags, OutWroteSomething);
		}
	}
}

template<typename PropertyType>
TArray<PropertyType*> FReplicableProperty::FindProperties(UStruct* Layout)
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

template<typename PropertyType>
TArray<FArrayProperty*> FReplicableProperty::FindArrayProperties(UStruct* Layout)
{
	TFieldIterator<FArrayProperty> Iterator(Layout);
	TArray<FArrayProperty*> OutProperties;
	for(; Iterator; ++Iterator)
	{
		FArrayProperty* Property = *Iterator;

		const bool bCheckReplicationFlags = (Property->GetPropertyFlags() & EPropertyFlags::CPF_Net) || (Property->GetPropertyFlags() & EPropertyFlags::CPF_RepNotify);
		const bool bCheckInnerType = (CastField<PropertyType>(Property->Inner) != nullptr);
		const bool bCheckReplication = (Cast<UClass>(Layout) != nullptr);
		
		if(bCheckInnerType && (!bCheckReplication || bCheckReplicationFlags))
		{
			OutProperties.Add(Property);
		}
	}

	return OutProperties;
}

void FReplicableProperty::ReplicateObjectProperty(void* Owner, const FObjectProperty* ObjectProperty, UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const
{
	UObject* Object = *ObjectProperty->ContainerPtrToValuePtr<UObject*>(Owner);
	ReplicateObject(Object, Channel, Bunch, RepFlags, OutWroteSomething);
}

void FReplicableProperty::ReplicateArrayObjectProperty(void* Owner, const FArrayProperty* ArrayObjectProperty, UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const
{
	TArray<UObject*> Array = *ArrayObjectProperty->ContainerPtrToValuePtr<TArray<UObject*>>(Owner);
	for(UObject* Object : Array)
	{
		ReplicateObject(Object, Channel, Bunch, RepFlags, OutWroteSomething);
	}
}

void FReplicableProperty::ReplicateObject(UObject* Object, UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags, bool& OutWroteSomething) const
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
	UObject* Outer = this->GetOuter();
	Graph->Replicate(Outer, Channel, Bunch, RepFlags, OutWroteSomething);
}

void UReplicator::BuildReplicationGraph()
{
	UClass* OuterClass = this->GetOuter()->GetClass();
	Graph = TUniquePtr<FReplicableProperty>(new FReplicableProperty(nullptr, OuterClass));
}
