// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/PartialSaveGame.h"

#include "Kismet/GameplayStatics.h"
#include "SaveSystem/SaveableInterface.h"
#include "SaveSystem/SaveGameArchive.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/MemoryReader.h"


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
            bool IsValid = false;
            LoadRecord(World, ObjectIndex, IsValid);
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
    // TODO: Refactor this place.
    if (IsValid(Object) && SavedObjects.Find(Object) == INDEX_NONE && (IsValid(Cast<AActor>(Object)) || IsValid(Cast<UActorComponent>(Object)) || Object->Implements<USaveableInterface>()))
    {
        if (IsValid(Cast<AActor>(Object)) || OutersForSaving.Find(Object->GetOuter()) != INDEX_NONE)
        {
            if (Object->Implements<USaveableInterface>())
            {
                ISaveableInterface::Execute_BeforeSave(Object);
            }

            FObjectRecord ObjectRecord = FObjectRecord();

            SaveObjectPart(Object, ObjectRecord);

            AActor* Actor = Cast<AActor>(Object);
            SaveActorPart(Actor, ObjectRecord);
            
            UActorComponent* ActorComponent = Cast<UActorComponent>(Object);
            SaveComponentPart(ActorComponent, ObjectRecord);

            SavedObjects.Add(Object);
            ObjectRecords.Add(ObjectRecord);
            AddOuterForSaving(Object);
            SaveSubobjects(Object, ObjectRecords.Num() - 1);

            if (Object->Implements<USaveableInterface>())
            {
                ISaveableInterface::Execute_AfterSave(Object);
            }
        }
    }
    else
    {
        FObjectRecord PointerOnAlreadySavedObject = FObjectRecord(EObjectType::Pointer, Object);
        ObjectRecords.Add(PointerOnAlreadySavedObject);
    }
}

void UPartialSaveGame::SaveObjectPart(UObject* Object, FObjectRecord& ObjectRecord)
{
    if (IsValid(Object))
    {
        TArray<uint8> ObjectData = SerializeObject(Object);
        ObjectRecord = FObjectRecord(EObjectType::Object, Object, Object->GetName(), Object->GetClass(), Object->GetOuter(), ObjectData);
    }
}

void UPartialSaveGame::SaveActorPart(AActor* Actor, FObjectRecord& ObjectRecord)
{
    if (IsValid(Actor))
    {
        ObjectRecord.Type = EObjectType::Actor;
        ObjectRecord.Transform = Actor->GetActorTransform();
        ObjectRecord.OwnerID = reinterpret_cast<int64>(Actor->GetOwner());

        if (IsValid(Actor->GetRootComponent()->GetAttachParent()))
        {
            ObjectRecord.ParentID = reinterpret_cast<int64>(Actor->GetRootComponent()->GetAttachParent());
            ObjectRecord.Socket = Actor->GetRootComponent()->GetAttachSocketName();
        }
    }
}

void UPartialSaveGame::SaveComponentPart(UActorComponent* ActorComponent, FObjectRecord& ObjectRecord)
{
    if(IsValid(ActorComponent))
    {
        ObjectRecord.Type = EObjectType::ActorComponent;
        ObjectRecord.OwnerID = reinterpret_cast<int64>(ActorComponent->GetOwner());

        USceneComponent* SceneComponent = Cast<USceneComponent>(ActorComponent);
        if (IsValid(SceneComponent))
        {
            ObjectRecord.ParentID = reinterpret_cast<int64>(SceneComponent->GetAttachParent());
            ObjectRecord.Transform = SceneComponent->GetComponentTransform();
            ObjectRecord.Socket = SceneComponent->GetAttachSocketName();
        }
    }
}

void UPartialSaveGame::SaveSubobjects(UObject* Object, const int64 ObjectRecordIndex)
{
    AActor* Actor = Cast<AActor>(Object);
    SaveSubobjectsOfActor(Actor, ObjectRecordIndex);

    USceneComponent* SceneComponent = Cast<USceneComponent>(Object);
    SaveSubobjectsOfSceneComponent(SceneComponent, ObjectRecordIndex);

    SaveSubobjectsOfObject(Object, ObjectRecordIndex);
}

void UPartialSaveGame::SaveSubobjectsOfObject(UObject* Object, const int64 ObjectRecordIndex)
{
    if (IsValid(Object))
    {
        UClass* Class = Object->GetClass();
	    SaveProperties(Object, Class, ObjectRecordIndex);
    }
}

void UPartialSaveGame::SaveSubobjectsOfActor(AActor* Actor, const int64 ObjectRecordIndex)
{
    if (IsValid(Actor))
    {
        TArray<UActorComponent*> Components;
        Actor->GetComponents(Components, false);
        ObjectRecords[ObjectRecordIndex].CountOfComponents = Components.Num();
        for (UActorComponent* Component : Components)
        {
            SaveObject(Component);
        }
    }
}

void UPartialSaveGame::SaveSubobjectsOfSceneComponent(USceneComponent* SceneComponent, const int64 ObjectRecordIndex)
{
    if (IsValid(SceneComponent))
    {
        TArray<USceneComponent*> Components = SceneComponent->GetAttachChildren();

        // Save attached components.
        TArray<USceneComponent*> AttachedComponents = Components.FilterByPredicate([SceneComponent](USceneComponent* const& ChildComponent){ return (SceneComponent->GetOwner() == ChildComponent->GetOwner()); });
        ObjectRecords[ObjectRecordIndex].CountOfComponents = AttachedComponents.Num();
        for (USceneComponent* Component : AttachedComponents)
        {
            SaveObject(Component);
        }

        // Save attached actors.
        TArray<USceneComponent*> AttachedRootComponents = Components.FilterByPredicate([SceneComponent](USceneComponent* const& ChildComponent){ return (SceneComponent->GetOwner() != ChildComponent->GetOwner()); });
        ObjectRecords[ObjectRecordIndex].CountOfAttachedActors = AttachedRootComponents.Num();
        for (USceneComponent* RootComponent : AttachedRootComponents)
        {
            SaveObject(RootComponent->GetOwner());
        }
    }
}

TArray<uint8> UPartialSaveGame::SerializeObject(UObject* Object)
{
    TArray<uint8> Data;
    FMemoryWriter MemoryWriter = FMemoryWriter(Data, true);
    FSaveGameArchive SaveGameArchive = FSaveGameArchive(MemoryWriter);
    Object->Serialize(SaveGameArchive);
    return Data;
}

void UPartialSaveGame::SaveProperties(void* Object, UStruct* Layout, const int64 ObjectRecordIndex)
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
        ObjectRecords[ObjectRecordIndex].SizesOfArrays.Add(Array.Num());
        for (UObject* Subobject : Array)
		{
            SaveObject(Subobject);
        }
    }

	TArray<FStructProperty*> FoundStructProperties = FindProperties<FStructProperty>(Layout);
	for (FStructProperty* FoundStructProperty : FoundStructProperties)
	{
		void* Subobject = FoundStructProperty->ContainerPtrToValuePtr<void*>(Object);
		SaveProperties(Subobject, FoundStructProperty->Struct, ObjectRecordIndex);
    }

	TArray<FArrayProperty*> FoundArrayStructProperties = FindArrayProperties<FStructProperty>(Layout);
	for (FArrayProperty* FoundArrayStructProperty : FoundArrayStructProperties)
	{
		FScriptArrayHelper_InContainer ArrayHelper = FScriptArrayHelper_InContainer(FoundArrayStructProperty, Object);
		for (int32 ArrayIndex = 0; ArrayIndex < ArrayHelper.Num(); ++ArrayIndex)
		{
			void* Subobject = ArrayHelper.GetRawPtr(ArrayIndex);
            SaveProperties(Subobject, CastField<FStructProperty>(FoundArrayStructProperty->Inner)->Struct, ObjectRecordIndex);
		}
    }
}

UObject* UPartialSaveGame::LoadRecord(UWorld* World, int64& ObjectIndex, bool& IsValid)
{
    ++ObjectIndex;
    FObjectRecord ObjectRecord = ObjectRecords[ObjectIndex];

    IsValid = true;
    UObject* Object = nullptr;
    switch (ObjectRecord.Type)
    {
        case EObjectType::Pointer:
        {
            if (LoadedObjects.Find(ObjectRecord.SelfID))
            {
                return LoadedObjects[ObjectRecord.SelfID];
            }
            
            IsValid = false;
            return nullptr;
        }
        break;

        case EObjectType::Object:
        {
            Object = LoadObject(World, ObjectRecord);
        }
        break;

        case EObjectType::Actor:
        {
            Object = LoadActor(World, ObjectRecord);
        }
        break;

        case EObjectType::ActorComponent:
        {
            Object = LoadComponent(World, ObjectRecord);
        }
        break;
        
        default:
        {
            IsValid = false;
            return nullptr;
        }
        break;
    }

    DeserializeObject(Object, ObjectRecord.Data);

    LoadedObjects.Add(ObjectRecord.SelfID, Object);

    AddOuterForLoading(Object);
    LoadSubobjects(Object, ObjectIndex);

    if (Object->Implements<USaveableInterface>())
    {
        ISaveableInterface::Execute_AfterLoad(Object);
    }

    return Object;
}

UObject* UPartialSaveGame::LoadObject(UWorld* World, const FObjectRecord& ObjectRecord)
{
    UObject* Object = NewObject<UObject>(OutersForLoading[ObjectRecord.OuterID], ObjectRecord.Class, ObjectRecord.Name);
    if (Object->Implements<USaveableInterface>())
    {
        ISaveableInterface::Execute_BeforeLoad(Object);
    }

    return Object;
}

AActor* UPartialSaveGame::LoadActor(UWorld* World, const FObjectRecord& ObjectRecord)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = ObjectRecord.Name;
    AActor* Owner = nullptr;
    if (ObjectRecord.OwnerID != reinterpret_cast<int64>(nullptr))
    {
        Owner = Cast<AActor>(LoadedObjects[ObjectRecord.OwnerID]);
    }
    AActor* Actor = World->SpawnActorDeferred<AActor>(ObjectRecord.Class, ObjectRecord.Transform, Owner);

    if (Actor->Implements<USaveableInterface>())
    {
        ISaveableInterface::Execute_BeforeLoad(Actor);
    }

    Actor->FinishSpawning(ObjectRecord.Transform);

    if (ObjectRecord.ParentID != reinterpret_cast<int64>(nullptr))
    {
        USceneComponent* SceneComponent = Cast<USceneComponent>(LoadedObjects[ObjectRecord.ParentID]);
        if (IsValid(SceneComponent))
        {
            FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
            Actor->AttachToComponent(SceneComponent, AttachmentRules, ObjectRecord.Socket);
        }
    }
    
    return Actor;
}

UActorComponent* UPartialSaveGame::LoadComponent(UWorld* World, const FObjectRecord& ObjectRecord)
{
    AActor* Owner = Cast<AActor>(OutersForLoading[ObjectRecord.OwnerID]);
    TArray<UActorComponent*> Components;
    Owner->GetComponents(Components, true);

    UActorComponent* ActorComponent = nullptr;
    UActorComponent** FoundComponent = Components.FindByPredicate([ObjectRecord](UActorComponent*& ActorComponent){ return (FName(ActorComponent->GetName()) == ObjectRecord.Name);});
    if (FoundComponent != nullptr)
    {
        ActorComponent = *FoundComponent;
    }

    if (!IsValid(ActorComponent))
    {
        ActorComponent = NewObject<UActorComponent>(Owner, ObjectRecord.Class, ObjectRecord.Name);

        USceneComponent* SceneComponent = Cast<USceneComponent>(ActorComponent);
        if (IsValid(SceneComponent))
        {
            USceneComponent* ParentComponent = Cast<USceneComponent>(LoadedObjects[ObjectRecord.ParentID]);
            SceneComponent->SetupAttachment(ParentComponent, ObjectRecord.Socket);
        }

        ActorComponent->RegisterComponent();
    }

    if (ActorComponent->Implements<USaveableInterface>())
    {
        ISaveableInterface::Execute_BeforeLoad(ActorComponent);
    }

    return ActorComponent;
}

void UPartialSaveGame::LoadSubobjects(UObject* Object, int64& ObjectIndex)
{
    const FObjectRecord& ObjectRecord = ObjectRecords[ObjectIndex];

    AActor* Actor = Cast<AActor>(Object);
    LoadSubobjectsOfActor(Actor, ObjectIndex, ObjectRecord);

    USceneComponent* SceneComponent = Cast<USceneComponent>(Object);
    LoadSubobjectsOfComponent(SceneComponent, ObjectIndex, ObjectRecord);

    LoadSubobjectsOfObject(Object, ObjectIndex, ObjectRecord);
}

void UPartialSaveGame::LoadSubobjectsOfObject(UObject* Object, int64& ObjectIndex, const FObjectRecord& ObjectRecord)
{
    UClass* Class = Object->GetClass();
    int64 ArrayIndex = -1;
    LoadProperties(Object->GetWorld(), Object, Class, ObjectIndex, ArrayIndex, ObjectRecord);
}

void UPartialSaveGame::LoadSubobjectsOfActor(AActor* Actor, int64& ObjectIndex, const FObjectRecord& ObjectRecord)
{
    if (IsValid(Actor))
    {
        for (int64 CountOfLoadedComponents = 0; CountOfLoadedComponents < ObjectRecord.CountOfComponents; ++CountOfLoadedComponents)
        {
            bool IsValid = false;
            LoadRecord(Actor->GetWorld(), ObjectIndex, IsValid);
        }
    }
}

void UPartialSaveGame::LoadSubobjectsOfComponent(USceneComponent* SceneComponent, int64& ObjectIndex, const FObjectRecord& ObjectRecord)
{
    if (IsValid(SceneComponent))
    {
        for (int64 CountOfLoadedComponents = 0; CountOfLoadedComponents < ObjectRecord.CountOfComponents; ++CountOfLoadedComponents)
        {
            bool IsValid = false;
            LoadRecord(SceneComponent->GetWorld(), ObjectIndex, IsValid);
        }

        for (int64 CountOfLoadedActors = 0; CountOfLoadedActors < ObjectRecord.CountOfAttachedActors; ++CountOfLoadedActors)
        {
            bool IsValid = false;
            LoadRecord(SceneComponent->GetWorld(), ObjectIndex, IsValid);
        }
    }
}

void UPartialSaveGame::DeserializeObject(UObject* Object, const TArray<uint8>& Data)
{
    FMemoryReader MemoryReader(Data, true);
    FSaveGameArchive SaveGameArchive = FSaveGameArchive(MemoryReader);
    Object->Serialize(SaveGameArchive);
}

void UPartialSaveGame::LoadProperties(UWorld* World, void* Object, UStruct* Layout, int64& ObjectIndex, int64& ArrayIndex, const FObjectRecord& ObjectRecord)
{
    TArray<FObjectProperty*> ObjectProperties = FindProperties<FObjectProperty>(Layout);
    for (FObjectProperty* ObjectProperty : ObjectProperties)
	{
        bool IsValid = false;
        UObject* Subobject = LoadRecord(World, ObjectIndex, IsValid);
        if (IsValid)
        {
		    ObjectProperty->SetObjectPropertyValue(ObjectProperty->ContainerPtrToValuePtr<UObject>(Object), Subobject);
        }
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
            bool IsValid = false;
			UObject* Subobject = LoadRecord(World, ObjectIndex, IsValid);
            if (IsValid)
            {
                Array.Add(Subobject);
            }
        }
    }

    TArray<FStructProperty*> FoundStructProperties = FindProperties<FStructProperty>(Layout);
	for (FStructProperty* FoundStructProperty : FoundStructProperties)
	{
        void* Subobject = FoundStructProperty->ContainerPtrToValuePtr<void*>(Object);
		LoadProperties(World, Subobject, FoundStructProperty->Struct, ObjectIndex, ArrayIndex, ObjectRecord);
    }

	TArray<FArrayProperty*> FoundArrayStructProperties = FindArrayProperties<FStructProperty>(Layout);
	for (FArrayProperty* FoundArrayStructProperty : FoundArrayStructProperties)
	{
		FScriptArrayHelper_InContainer ArrayHelper = FScriptArrayHelper_InContainer(FoundArrayStructProperty, Object);
		for (int32 IndexInArray = 0; IndexInArray < ArrayHelper.Num(); ++IndexInArray)
		{
			void* Subobject = ArrayHelper.GetRawPtr(IndexInArray);
            LoadProperties(World, Subobject, CastField<FStructProperty>(FoundArrayStructProperty->Inner)->Struct, ObjectIndex, ArrayIndex, ObjectRecord);
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
