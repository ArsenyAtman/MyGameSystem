// Fill out your copyright notice in the Description page of Project Settings.


#include "PartialSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SaveableInterface.h"
#include "SaveGameArchive.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/MemoryReader.h"
#include "SaveableInterface.h"


void UPartialSaveGame::SetOutersForSaving(const TArray<UObject*>& Outers)
{
    OutersForSaving = Outers;

    OuterIDs.Empty();
    for (UObject* Outer : OutersForSaving)
    {
        OuterIDs.Add(reinterpret_cast<int64>(Outer));
    }
}

void UPartialSaveGame::Save(const TArray<UObject*> Objects)
{
    for (UObject* Object : Objects)
    {
        SaveObject(Object);
    }
}

void UPartialSaveGame::SetOutersForLoading(const TArray<UObject*>& Outers)
{
    for (int i = 0; i < Outers.Num(); ++i)
    {
        OutersForLoading.Add(OuterIDs[i], Outers[i]);
    }
}

void UPartialSaveGame::Load(const UObject* WorldContextObject)
{
    UWorld* World = WorldContextObject->GetWorld();
    if (IsValid(World))
    {
        int64 ObjectIndex = -1;
        const int64 MaxObjectIndex = ObjectRecords.Num() - 1;
        while (ObjectIndex < MaxObjectIndex)
        {
            ++ObjectIndex;
            LoadObject(World, ObjectIndex);
        }
    }
}

void UPartialSaveGame::AddOuterForSaving(UObject* Outer)
{
    OutersForSaving.Add(Outer);
    OuterIDs.Add(reinterpret_cast<int64>(Outer));
}

void UPartialSaveGame::AddOuterForLoading(UObject* Outer)
{
    OutersForLoading.Add(OuterIDs[OutersForLoading.Num()], Outer);
}

void UPartialSaveGame::SaveObject(UObject* Object)
{
    if (SavedObjects.Find(Object) == INDEX_NONE)//OutersForSaving.Find(Object->GetOuter()) != INDEX_NONE)
    {
        TArray<uint8> ObjectData = SerializeObject(Object);
        FObjectRecord ObjectRecord = FObjectRecord(Object, Object->GetName(), Object->GetClass(), Object->GetOuter(), ObjectData);
        ObjectRecord.Type = EObjectType::Object;

        TArray<FArrayProperty*> ArrayObjectProperties = FindArrayProperties<FObjectProperty>(Object->GetClass());
        for (FArrayProperty* ArrayObjectProperty : ArrayObjectProperties)
        {
            TArray<UObject*> Array = *ArrayObjectProperty->ContainerPtrToValuePtr<TArray<UObject*>>(Object);
            
        }

        AActor* Actor = Cast<AActor>(Object);
        if (IsValid(Actor))
        {
            ObjectRecord.Type = EObjectType::Actor;
            ObjectRecord.Transform = Actor->GetActorTransform();

            TArray<UActorComponent*> Components;
            Actor->GetComponents(Components, false);
            ObjectRecord.CountOfComponents = Components.Num();
        }
        
        UActorComponent* ActorComponent = Cast<UActorComponent>(Object);
        if (IsValid(ActorComponent))
        {
            ObjectRecord.Type = EObjectType::ActorComponent;
            ObjectRecord.OwnerID = reinterpret_cast<int64>(ActorComponent->GetOwner());
            
            USceneComponent* SceneComponent = Cast<USceneComponent>(ActorComponent);
            if (IsValid(SceneComponent))
            {
                ObjectRecord.ParentID = reinterpret_cast<int64>(SceneComponent->GetAttachParent());
                ObjectRecord.Transform = SceneComponent->GetComponentTransform();
                ObjectRecord.CountOfComponents = SceneComponent->GetAttachChildren().Num();
            }
        }

        SavedObjects.Add(Object);
        ObjectRecords.Add(ObjectRecord);
        AddOuterForSaving(Object);
        SaveSubobjects(Object, ObjectRecords.Num() - 1);
    }
    else
    {
        ObjectRecords.Add(FObjectRecord(Object));
    }
}

void UPartialSaveGame::SaveSubobjects(UObject* Object, int64 ObjectIndex)
{    
    AActor* Actor = Cast<AActor>(Object);
    if (IsValid(Actor))
    {
        TArray<UActorComponent*> Components;
        Actor->GetComponents(Components, false);
        for (UActorComponent* Component : Components)
        {
            SaveObject(Component);
        }
    }

    USceneComponent* SceneComponent = Cast<USceneComponent>(Object);
    if (IsValid(SceneComponent))
    {
        TArray<USceneComponent*> Components = SceneComponent->GetAttachChildren();
        for (USceneComponent* Component : Components)
        {
            SaveObject(Component);
        }
    }

    UClass* Class = Object->GetClass();
	SaveStructures(Object, Class, ObjectIndex);
}

TArray<uint8> UPartialSaveGame::SerializeObject(UObject* Object)
{
    TArray<uint8> Data;
    FMemoryWriter MemoryWriter = FMemoryWriter(Data, true);
    FSaveGameArchive SaveGameArchive = FSaveGameArchive(MemoryWriter);
    Object->Serialize(SaveGameArchive);
    return Data;
}

UObject* UPartialSaveGame::LoadObject(UWorld* World, int64& ObjectIndex)
{
    FObjectRecord ObjectRecord = ObjectRecords[ObjectIndex];

    if (ObjectRecord.OuterID == reinterpret_cast<int64>(nullptr) && LoadedObjects.Find(ObjectRecord.SelfID))
    {
        return LoadedObjects[ObjectRecord.SelfID];
    }

    UObject* Object = nullptr;

    switch (ObjectRecord.Type)
    {
        case EObjectType::Actor:
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Name = ObjectRecord.Name;
            // TODO: Find the outer.
            AActor* Actor = World->SpawnActorDeferred<AActor>(ObjectRecord.Class, ObjectRecord.Transform, nullptr);
            Actor->FinishSpawning(ObjectRecord.Transform);
            
            Object = Actor;
        }
        break;

        case EObjectType::ActorComponent:
        {
            AActor* Owner = Cast<AActor>(OutersForLoading[ObjectRecord.OwnerID]);
            TArray<UActorComponent*> Components;
            Owner->GetComponents(Components, true);

            Object = *Components.FindByPredicate([ObjectRecord](UActorComponent*& ActorComponent){ return (FName(ActorComponent->GetName()) == ObjectRecord.Name);});//Owner->GetDefaultSubobjectByName(ObjectRecord.Name);

            if (!IsValid(Object))
            {
                UActorComponent* ActorComponent = NewObject<UActorComponent>(Owner, ObjectRecord.Class, ObjectRecord.Name);
                if (IsValid(ActorComponent))
                {
                    Object = ActorComponent;

                    USceneComponent* SceneComponent = Cast<USceneComponent>(Object);
                    if (IsValid(SceneComponent))
                    {
                        USceneComponent* ParentComponent = Cast<USceneComponent>(OutersForLoading[ObjectRecord.ParentID]);
                        SceneComponent->SetupAttachment(ParentComponent);
                    }

                    ActorComponent->RegisterComponent();
                }
            }
        }
        break;

        case EObjectType::Object:
        {
            Object = NewObject<UObject>(OutersForLoading[ObjectRecord.OuterID], ObjectRecord.Class, ObjectRecord.Name);
        }
        break;
        
        default:
        {
            return nullptr;
        }
        break;
    }

    DeserializeObject(Object, ObjectRecord.Data);

    LoadedObjects.Add(ObjectRecord.SelfID, Object);

    AddOuterForLoading(Object);
    LoadSubobjects(Object, ObjectIndex);

    AActor* Actor = Cast<AActor>(Object);
    if (IsValid(Actor))
    {
        //Actor->FinishSpawning(ObjectRecord.Transform);
    }

    UActorComponent* ActorComponent = Cast<UActorComponent>(Object);
    if (IsValid(ActorComponent))
    {
        //ActorComponent->RegisterComponent();
    }

    return Object;
}

void UPartialSaveGame::LoadSubobjects(UObject* Object, int64& ObjectIndex)
{
    FObjectRecord ObjectRecord = ObjectRecords[ObjectIndex];

    AActor* Actor = Cast<AActor>(Object);
    if (IsValid(Actor))
    {
        for (int64 CountOfLoadedComponents = 0; CountOfLoadedComponents < ObjectRecord.CountOfComponents; ++CountOfLoadedComponents)
        {
            ++ObjectIndex;
            LoadObject(Object->GetWorld(), ObjectIndex);
        }
    }

    USceneComponent* SceneComponent = Cast<USceneComponent>(Object);
    if (IsValid(SceneComponent))
    {
        for (int64 CountOfLoadedComponents = 0; CountOfLoadedComponents < ObjectRecord.CountOfComponents; ++CountOfLoadedComponents)
        {
            ++ObjectIndex;
            LoadObject(Object->GetWorld(), ObjectIndex);
        }
    }

    UClass* Class = Object->GetClass();
    int64 ArrayIndex = -1;
    LoadStructures(Object->GetWorld(), Object, Class, ObjectIndex, ArrayIndex, ObjectRecord);
}

void UPartialSaveGame::DeserializeObject(UObject* Object, const TArray<uint8>& Data)
{
    FMemoryReader MemoryReader(Data, true);
    FSaveGameArchive SaveGameArchive = FSaveGameArchive(MemoryReader);
    Object->Serialize(SaveGameArchive);
}

void UPartialSaveGame::SaveStructures(void* Object, UStruct* Layout, int64 ObjectIndex)
{
    TArray<FObjectProperty*> ObjectProperties = FindProperties<FObjectProperty>(Layout);
    for (FObjectProperty* ObjectProperty : ObjectProperties)
	{
		UObject* Subobject = ObjectProperty->GetObjectPropertyValue(ObjectProperty->ContainerPtrToValuePtr<UObject>(Object));
        SaveObject(Subobject);
	}

    TArray<FArrayProperty*> ArrayObjectProperties = FindArrayProperties<FObjectProperty>(Layout);
    for (FArrayProperty* ArrayObjectProperty : ArrayObjectProperties)
    {
        TArray<UObject*> Array = *ArrayObjectProperty->ContainerPtrToValuePtr<TArray<UObject*>>(Object);
        ObjectRecords[ObjectIndex].SizesOfArrays.Add(Array.Num());
        for (UObject* Subobject : Array)
		{
            SaveObject(Subobject);
        }
    }

	TArray<FStructProperty*> FoundStructProperties = FindProperties<FStructProperty>(Layout);
	for (FStructProperty* FoundStructProperty : FoundStructProperties)
	{
		void* Subobject = FoundStructProperty->ContainerPtrToValuePtr<void*>(Object);
		SaveStructures(Subobject, FoundStructProperty->Struct, ObjectIndex);
    }

	TArray<FArrayProperty*> FoundArrayStructProperties = FindArrayProperties<FStructProperty>(Layout);
	for (FArrayProperty* FoundArrayStructProperty : FoundArrayStructProperties)
	{
		FScriptArrayHelper_InContainer ArrayHelper = FScriptArrayHelper_InContainer(FoundArrayStructProperty, Object);
        //ObjectRecord.SizesOfArrays.Add(ArrayHelper.Num());
		for (int32 ArrayIndex = 0; ArrayIndex < ArrayHelper.Num(); ++ArrayIndex)
		{
			void* Subobject = ArrayHelper.GetRawPtr(ArrayIndex);
            SaveStructures(Subobject, CastField<FStructProperty>(FoundArrayStructProperty->Inner)->Struct, ObjectIndex);
		}
    }
}

void UPartialSaveGame::LoadStructures(UWorld* World, void* Object, UStruct* Layout, int64& ObjectIndex, int64& ArrayIndex, FObjectRecord& ObjectRecord)
{
    TArray<FObjectProperty*> ObjectProperties = FindProperties<FObjectProperty>(Layout);
    for (FObjectProperty* ObjectProperty : ObjectProperties)
	{
        ++ObjectIndex;
        UObject* Subobject = LoadObject(World, ObjectIndex);
		ObjectProperty->SetObjectPropertyValue(ObjectProperty->ContainerPtrToValuePtr<UObject>(Object), Subobject);
	}

    TArray<FArrayProperty*> ArrayObjectProperties = FindArrayProperties<FObjectProperty>(Layout);
    for (int32 Index = 0; Index < ArrayObjectProperties.Num(); ++Index)
    {
        ++ArrayIndex;
        int32 SizeOfArray = ObjectRecord.SizesOfArrays[ArrayIndex];
        FArrayProperty* ArrayObjectProperty = ArrayObjectProperties[Index];
        TArray<UObject*>& Array = *ArrayObjectProperty->ContainerPtrToValuePtr<TArray<UObject*>>(Object);
        Array.Empty();
        for (int32 IndexInArray = 0; IndexInArray < SizeOfArray; ++IndexInArray)
		{
            ++ObjectIndex;
			UObject* Subobject = LoadObject(World, ObjectIndex);
            Array.Add(Subobject);
        }
    }

    TArray<FStructProperty*> FoundStructProperties = FindProperties<FStructProperty>(Layout);
	for (FStructProperty* FoundStructProperty : FoundStructProperties)
	{
        void* Subobject = FoundStructProperty->ContainerPtrToValuePtr<void*>(Object);
		LoadStructures(World, Subobject, FoundStructProperty->Struct, ObjectIndex, ArrayIndex, ObjectRecord);
    }

	TArray<FArrayProperty*> FoundArrayStructProperties = FindArrayProperties<FStructProperty>(Layout);
	for (FArrayProperty* FoundArrayStructProperty : FoundArrayStructProperties)
	{
		FScriptArrayHelper_InContainer ArrayHelper = FScriptArrayHelper_InContainer(FoundArrayStructProperty, Object);
		for (int32 IndexInArray = 0; IndexInArray < ArrayHelper.Num(); ++IndexInArray)
		{
			void* Subobject = ArrayHelper.GetRawPtr(IndexInArray);
            LoadStructures(World, Subobject, CastField<FStructProperty>(FoundArrayStructProperty->Inner)->Struct, ObjectIndex, ArrayIndex, ObjectRecord);
		}
    }
}

template<typename PropertyType>
TArray<PropertyType*> UPartialSaveGame::FindProperties(UStruct* Layout)
{
	TFieldIterator<PropertyType> Iterator(Layout);
	TArray<PropertyType*> OutProperties;
	for(; Iterator; ++Iterator)
	{
		PropertyType* Property = *Iterator;
		if(Property->GetPropertyFlags() & EPropertyFlags::CPF_SaveGame)
		{
			OutProperties.Add(Property);
		}
	}

	return OutProperties;
}

template<typename ElementType>
TArray<FArrayProperty*> UPartialSaveGame::FindArrayProperties(UStruct* Layout)
{
	TFieldIterator<FArrayProperty> Iterator(Layout);
	TArray<FArrayProperty*> OutProperties;
	for(; Iterator; ++Iterator)
	{
		FArrayProperty* Property = *Iterator;
		if((CastField<ElementType>(Property->Inner) != nullptr) && (Property->GetPropertyFlags() & EPropertyFlags::CPF_SaveGame))
		{
			OutProperties.Add(Property);
		}
	}

	return OutProperties;
}
