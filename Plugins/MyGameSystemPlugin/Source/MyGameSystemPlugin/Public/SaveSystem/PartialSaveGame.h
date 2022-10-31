// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ObjectRecord.h"
#include "PartialSaveGame.generated.h"

class FObjectProperty;
class FArrayProperty;

/**
 * A SaveGame that saves provided UObjects (all properties for saving in this objects (and their subbojects and substructs) need to be marked as SaveGame).
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEMPLUGIN_API UPartialSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	/**
	 * Set array of outers for saving objects.
	 * @param Outers - A set of outers.
	 */
	UFUNCTION(BlueprintCallable)
	void SetOutersForSaving(const TArray<UObject*>& Outers);

	/**
	 * Save objects.
	 * @param Objects - Objects to save.
	 */
	UFUNCTION(BlueprintCallable)
	void Save(const TArray<UObject*> Objects);

	/**
	 * Set array of outers for loading objects.
	 * @param Outers - A set of outers.
	 */
	UFUNCTION(BlueprintCallable)
	void SetOutersForLoading(const TArray<UObject*>& Outers);

	/**
	 * Load saved objects.
	 * @param WorldContextObject - An object in a world.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	void Load(const UObject* WorldContextObject);

private:

	void AddOuterForSaving(UObject* Outer);
	void AddOuterForLoading(UObject* Outer);

	void SaveObject(UObject* Object);
	void SaveObjectPart(UObject* Object, FObjectRecord& ObjectRecord);
	void SaveActorPart(AActor* Actor, FObjectRecord& ObjectRecord);
	void SaveComponentPart(UActorComponent* Component, FObjectRecord& ObjectRecord);

	void SaveSubobjects(UObject* Object, const int64 ObjectRecordIndex);
	void SaveSubobjectsOfObject(UObject* Object, const int64 ObjectRecordIndex);
	void SaveSubobjectsOfActor(AActor* Actor, const int64 ObjectRecordIndex);
	void SaveSubobjectsOfSceneComponent(USceneComponent* Component, const int64 ObjectRecordIndex);

	TArray<uint8> SerializeObject(UObject* Object);

	void SaveProperties(void* Object, UStruct* Layout, const int64 ObjectRecordIndex);

	UObject* LoadRecord(UWorld* World, int64& ParentObjectRecordIndex, bool& IsValid);
	UObject* LoadObject(UWorld* World, const FObjectRecord& ObjectRecord);
	AActor* LoadActor(UWorld* World, const FObjectRecord& ObjectRecord);
	UActorComponent* LoadComponent(UWorld* World, const FObjectRecord& ObjectRecord);

	void LoadSubobjects(UObject* Object, int64& ObjectIndex);
	void LoadSubobjectsOfObject(UObject* Object, int64& ObjectIndex, const FObjectRecord& ObjectRecord);
	void LoadSubobjectsOfActor(AActor* Actor, int64& ObjectIndex, const FObjectRecord& ObjectRecord);
	void LoadSubobjectsOfComponent(USceneComponent* Component, int64& ObjectIndex, const FObjectRecord& ObjectRecord);

	void DeserializeObject(UObject* Object, const TArray<uint8>& Data);
	
	void LoadProperties(UWorld* World, void* Object, UStruct* Layout, int64& ObjectIndex, int64& ArrayIndex, const FObjectRecord& ObjectRecord);

	template<typename PropertyType>
	TArray<PropertyType*> FindProperties(UStruct* Layout);

	template<typename ElementType>
	TArray<FArrayProperty*> FindArrayProperties(UStruct* Layout);

	TArray<UObject*> OutersForSaving;
	TMap<int64, UObject*> OutersForLoading;

	TArray<UObject*> SavedObjects;
	TMap<int64, UObject*> LoadedObjects;

	UPROPERTY()
	TArray<int64> OuterIDs;

	UPROPERTY()
	TArray<FObjectRecord> ObjectRecords;
	
};
