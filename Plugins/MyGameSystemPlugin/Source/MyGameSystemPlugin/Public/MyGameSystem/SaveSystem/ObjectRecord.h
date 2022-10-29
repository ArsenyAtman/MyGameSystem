// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectRecord.generated.h"

UENUM()
enum class EObjectType : uint8
{
    Pointer             UMETA(DisplayName = "Pointer"),
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
    EObjectType Type;

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
    TArray<int32> SizesOfArrays = TArray<int32>();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 CountOfComponents = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 CountOfAttachedActors = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTransform Transform = FTransform();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName Socket = "None";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 OwnerID = reinterpret_cast<int64>(nullptr);

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
