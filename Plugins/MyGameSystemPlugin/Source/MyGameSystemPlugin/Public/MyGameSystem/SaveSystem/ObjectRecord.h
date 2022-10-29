// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectRecord.generated.h"

/**
 * An object type for the save system.
 */
UENUM()
enum class EObjectType : uint8
{
    Pointer             UMETA(DisplayName = "Pointer"),
    Object              UMETA(DisplayName = "Object"),
    Actor               UMETA(DisplayName = "Actor"),
    ActorComponent      UMETA(DisplayName = "ActorComponent")
};

/**
 * An object representation in the save system.
 */
USTRUCT(BlueprintType, Blueprintable)
struct FObjectRecord
{
    GENERATED_BODY()

public:

    /**
     * Object type for the save system.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EObjectType Type;

    /**
     * This object.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 SelfID;

    /**
     * Name of this object.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName Name;

    /**
     * Class of this object.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UClass* Class;

    /**
     * Outer for this object.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 OuterID;

    /**
     * Pure data that is being stored by values in this object.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<uint8> Data;

    /**
     * Size of the each array in this object.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> SizesOfArrays = TArray<int32>();

    /**
     * Count of components on this actor.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 CountOfComponents = 0;

    /**
     * Count of attached actors to this component.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 CountOfAttachedActors = 0;

    /**
     * Transform for actors and scene components.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTransform Transform = FTransform();

    /**
     * Socket for attachment of scene components and actors.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName Socket = "None";

    /**
     * Owner for components and actors.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 OwnerID = reinterpret_cast<int64>(nullptr);

    /**
     * Object that self is attached to.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 ParentID = reinterpret_cast<int64>(nullptr);

    FObjectRecord(
        EObjectType ObjectType = EObjectType::Pointer,
        UObject* Object = nullptr, 
        const FString& ObjectName = "None", 
        UClass* ObjectClass = nullptr, 
        UObject* ObjectOuter = nullptr, 
        const TArray<uint8>& ObjectData = TArray<uint8>()
        )
    {
        Type = ObjectType;
        SelfID = reinterpret_cast<int64>(Object);
        Name = FName(ObjectName);
        Class = ObjectClass;
        OuterID = reinterpret_cast<int64>(ObjectOuter);
        Data = ObjectData;
    }
};
