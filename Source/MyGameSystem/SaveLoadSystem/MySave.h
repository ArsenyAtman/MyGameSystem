
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ObjectRecord.h"

#include "MySave.generated.h"

UCLASS(Blueprintable)
class MYGAMESYSTEM_API UMySave : public USaveGame
{
    GENERATED_BODY()

public:

    // All object data in one array
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FObjectRecord> ObjectRecords;

    // used for temp loading objects before serializing but after loading
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UObject*> TempObjects;

    // outers that are part of the map or otherwise preloaded so won't be in the list of TempObjects
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UObject*> PersistentOuters;

public:

    // basically just a wrapper so you don't have to do a for loop in blueprints
    UFUNCTION(BlueprintCallable)
    void ActorArraySaver(TArray<AActor*>& SaveActors);

    // Save individual Actors
    UFUNCTION(BlueprintCallable)
    void ActorSaver(AActor* SaveActor);

    // Create all saved actors without any data serialized yet
    UFUNCTION(BlueprintCallable)
    void ActorPreloader(AActor* WorldActor, FObjectRecord& ActorRecord);

    // basically just a wrapper so you don't have to do a for loop in blueprints
    UFUNCTION(BlueprintCallable)
    void UObjectArraySaver(TArray<UObject*>& SaveObjects);

    // Save individual objects
    UFUNCTION(BlueprintCallable)
    void UObjectSaver(UObject* SaveObject);

    // create all saved objects without any data serialized yet
    UFUNCTION(BlueprintCallable)
    void UObjectsPreloader(AActor* WorldActor);

    // load all data after all objects exist so all pointers will load
    UFUNCTION(BlueprintCallable)
    void UObjectDataLoader();

    // serialize the data
    UFUNCTION(BlueprintCallable)
    void SaveData(UObject* Object, TArray<uint8>& Data);

    // de-serialize the data
    UFUNCTION(BlueprintCallable)
    void LoadData(UObject* Object, TArray<uint8>& Data);
};
