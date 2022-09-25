// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectRecord.generated.h"

UENUM()
enum class EObjectType : uint8
{
    None                UMETA(DisplayName = "None"),
    Object              UMETA(DisplayName = "Object"),
    Actor               UMETA(DisplayName = "Actor"),
    ActorComponent      UMETA(DisplayName = "ActorComponent")
};

USTRUCT(BlueprintType, Blueprintable)
struct FObjectRecord
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 SelfID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UClass* Class;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 OuterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<uint8> Data;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EObjectType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 CountOfComponents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTransform Transform;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 OwnerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 ParentID;

    FObjectRecord(
        UObject* Object = nullptr, 
        const FString& ObjectName = "None", 
        UClass* ObjectClass = nullptr, 
        UObject* ObjectOuter = nullptr, 
        const TArray<uint8>& ObjectData = TArray<uint8>(),
        EObjectType ObjectType = EObjectType::None, 
        int64 ObjectCountOfComponents = 0, 
        const FTransform& ObjectTransform = FTransform(), 
        UObject* ObjectOwner = nullptr,
        UObject* ObjectParent = nullptr
        )
    {
        SelfID = reinterpret_cast<int64>(Object);
        Name = FName(ObjectName);
        Class = ObjectClass;
        OuterID = reinterpret_cast<int64>(ObjectOuter);
        Data = ObjectData;
        Type = ObjectType;
        CountOfComponents = ObjectCountOfComponents;
        Transform = ObjectTransform;
        OwnerID = reinterpret_cast<int64>(ObjectOwner);
        ParentID = reinterpret_cast<int64>(ObjectParent);
    }
};
