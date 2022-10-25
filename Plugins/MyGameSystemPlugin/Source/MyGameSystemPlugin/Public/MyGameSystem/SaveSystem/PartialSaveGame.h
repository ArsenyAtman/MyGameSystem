// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ObjectRecord.h"
#include "PartialSaveGame.generated.h"

class FObjectProperty;
class FArrayProperty;

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEMPLUGIN_API UPartialSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetOutersForSaving(const TArray<UObject*>& Outers);

	UFUNCTION(BlueprintCallable)
	void Save(const TArray<UObject*> Objects);

	UFUNCTION(BlueprintCallable)
	void SetOutersForLoading(const TArray<UObject*>& Outers);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	void Load(const UObject* WorldContextObject);

private:

	// TODO: Support saveable interface.
	// TODO: Make protected interface (?)

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

	// TODO: pass the index of parent and increment it,
	UObject* LoadRecord(UWorld* World, int64& ObjectRecordIndex);
	UObject* LoadObject(UWorld* World, const FObjectRecord& ObjectRecord);
	AActor* LoadActor(UWorld* World, const FObjectRecord& ObjectRecord);
	UActorComponent* LoadComponent(UWorld* World, const FObjectRecord& ObjectRecord);

	void LoadSubobjects(UObject* Object, int64& ObjectIndex);
	void LoadSubobjectsOfObject(UObject* Object, int64& ObjectIndex, const FObjectRecord& ObjectRecord);
	void LoadSubobjectsOfActor(AActor* Actor, int64& ObjectIndex, const FObjectRecord& ObjectRecord);
	void LoadSubobjectsOfComponent(USceneComponent* Component, int64& ObjectIndex, const FObjectRecord& ObjectRecord);

	TArray<uint8> SerializeObject(UObject* Object);
	void DeserializeObject(UObject* Object, const TArray<uint8>& Data);

	void SaveProperties(void* Object, UStruct* Layout, const int64 ObjectRecordIndex);
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
