
#pragma once

#include "CoreMinimal.h"

#include "ObjectRecord.generated.h"

USTRUCT(BlueprintType)
struct FObjectRecord
{

    GENERATED_USTRUCT_BODY()

public:

    // class that this object is
    UPROPERTY(BlueprintReadWrite)
    UClass* Class;

    // save the outer used for object so they get loaded back in the correct hierachy
    UPROPERTY(BlueprintReadWrite)
    UObject* Outer;

    // save the outer used for object so they get loaded back in the correct hierachy
    UPROPERTY(BlueprintReadWrite)
    int32 OuterID;

    // if the outer is an actor otherwise will be UObject
    UPROPERTY(BlueprintReadWrite)
    bool bActor;

    // this is for loading only, store a pointer for the loaded object here so you can loop for the records later to de-serialize all the data
    UPROPERTY(BlueprintReadWrite)
    UObject* Self;

    // Name of the object
    UPROPERTY(BlueprintReadWrite)
    FName Name;

    // serialized data for all UProperties that are 'SaveGame' enabled
    UPROPERTY(BlueprintReadWrite)
    TArray<uint8> Data;

    // Spawn location if it's an actor
    UPROPERTY(BlueprintReadWrite)
    FTransform Transform;

    FObjectRecord()
    {
        Class = nullptr;
        Outer = nullptr;
        Self = nullptr;
    }
};
