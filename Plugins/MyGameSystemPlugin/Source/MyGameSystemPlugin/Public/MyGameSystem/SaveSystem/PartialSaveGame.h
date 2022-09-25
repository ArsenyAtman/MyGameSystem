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

protected:

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	void AddOuterForSaving(UObject* Outer);

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	void AddOuterForLoading(UObject* Outer);
	
	// void AddRewritable(UObject)

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	void SaveObject(UObject* Object);

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	void SaveSubobjects(UObject* Object);

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	TArray<uint8> SerializeObject(UObject* Object);

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	UObject* LoadObject(UWorld* World, int64& ObjectIndex);

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	void LoadSubobjects(UObject* Object, int64& ObjectIndex);

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	void DeserializeObject(UObject* Object, const TArray<uint8>& Data);

private:

	TArray<FObjectProperty*> FindObjectPropertiesWithSaveGame(TFieldIterator<FObjectProperty> ObjectsIterator);

	TArray<UObject*> OutersForSaving;
	TMap<int64, UObject*> OutersForLoading;

	TArray<UObject*> SavedObjects;
	TMap<int64, UObject*> LoadedObjects;

	UPROPERTY()
	TArray<int64> OuterIDs;

	UPROPERTY()
	TArray<FObjectRecord> ObjectRecords;

	UPROPERTY()
	TArray<FObjectRecord> ObjectRecordsForRewriting;
	
};
