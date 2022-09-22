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
        int64 ObjectIndex = 0;
        for (const FObjectRecord& ObjectRecord : ObjectRecords)
        {
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

        if (AActor* Actor = Cast<AActor>(Object))
        {
            ObjectRecord.Type = EObjectType::Actor;
            ObjectRecord.Transform = Actor->GetActorTransform();
        }
        else if (UActorComponent* ActorComponent = Cast<UActorComponent>(Object))
        {
            ObjectRecord.Type = EObjectType::ActorComponent;
            
            USceneComponent* SceneComponent = Cast<USceneComponent>(ActorComponent);
            if (IsValid(SceneComponent))
            {
                ObjectRecord.Transform = SceneComponent->GetComponentTransform();
            }
        }

        SavedObjects.Add(Object);
        ObjectRecords.Add(ObjectRecord);
        AddOuterForSaving(Object);
        SaveSubobjects(Object, ObjectRecord);
    }
    else
    {
        ObjectRecords.Add(FObjectRecord(Object));
    }
}

void UPartialSaveGame::SaveSubobjects(UObject* Object, FObjectRecord& ObjectRecord)
{
    UClass* Class = Object->GetClass();
	TFieldIterator<FObjectProperty> ObjectsIterator(Class);
	TArray<FObjectProperty*> ObjectProperties = FindObjectPropertiesWithSaveGame(ObjectsIterator);
    for (FObjectProperty* ObjectProperty : ObjectProperties)
	{
		UObject* Subobject = ObjectProperty->GetObjectPropertyValue(ObjectProperty->ContainerPtrToValuePtr<UObject>(Object));
        SaveObject(Subobject);
	}

    // TODO: Add arrays for saving

    // TODO: Add components for saving
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

    if (ObjectRecord.Type == EObjectType::Actor)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Name = ObjectRecord.Name;
        Object = World->SpawnActorDeferred<AActor>(ObjectRecord.Class, ObjectRecord.Transform, nullptr);
        DeserializeObject(Object, ObjectRecord.Data);
    }
    else
    {
        Object = NewObject<UObject>(OutersForLoading[ObjectRecord.OuterID], ObjectRecord.Class, ObjectRecord.Name);
        DeserializeObject(Object, ObjectRecord.Data);
    }

    if (IsValid(Object))
    {
        LoadedObjects.Add(ObjectRecord.SelfID, Object);
    }

    AddOuterForLoading(Object);
    LoadSubobjects(Object, ObjectIndex);

    AActor* Actor = Cast<AActor>(Object);
    if (IsValid(Actor))
    {
        Actor->FinishSpawning(ObjectRecord.Transform);
    }

    return Object;
}

void UPartialSaveGame::LoadSubobjects(UObject* Object, int64& ObjectIndex)
{
    UClass* Class = Object->GetClass();
	TFieldIterator<FObjectProperty> ObjectsIterator(Class);
	TArray<FObjectProperty*> ObjectProperties = FindObjectPropertiesWithSaveGame(ObjectsIterator);
    for (FObjectProperty* ObjectProperty : ObjectProperties)
	{
        ++ObjectIndex;
        UObject* Subobject = LoadObject(Object->GetWorld(), ObjectIndex);
		ObjectProperty->SetObjectPropertyValue(ObjectProperty->ContainerPtrToValuePtr<UObject>(Object), Subobject);
	}

    // TODO: Add arrays for loading

    // TODO: Add components for loading
}

void UPartialSaveGame::DeserializeObject(UObject* Object, const TArray<uint8>& Data)
{
    FMemoryReader MemoryReader(Data, true);
    FSaveGameArchive SaveGameArchive = FSaveGameArchive(MemoryReader);
    Object->Serialize(SaveGameArchive);
}

TArray<FObjectProperty*> UPartialSaveGame::FindObjectPropertiesWithSaveGame(TFieldIterator<FObjectProperty> ObjectsIterator)
{
    TArray<FObjectProperty*> OutProperties;
	for(; ObjectsIterator; ++ObjectsIterator)
	{
		FObjectProperty* ObjectProperty = *ObjectsIterator;
		if(ObjectProperty->GetPropertyFlags() & EPropertyFlags::CPF_SaveGame)
		{
			OutProperties.Add(ObjectProperty);
		}
	}

	return OutProperties;
}
