// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedObject.h"

void UAdvancedObject::Tick(float DeltaTime)
{
    ObjectTick(DeltaTime);
}

bool UAdvancedObject::IsTickable() const
{
    return bCanEverTick;
}

TStatId UAdvancedObject::GetStatId() const
{
    return TStatId();
}

void UAdvancedObject::PostInitProperties()
{
    Super::PostInitProperties();

    if(IsValid(GetFirstOuterActor()) && IsValid(GetFirstOuterActor()->GetWorld()))
    {
        BeginPlay();
    }
}

void UAdvancedObject::BeginDestroy()
{
    Super::BeginDestroy();

    if(IsValid(GetFirstOuterActor()) && IsValid(GetFirstOuterActor()->GetWorld()))
    {
        EndPlay();
    }
}

UWorld* UAdvancedObject::GetWorld() const
{
    if(IsValid(GetFirstOuterActor()))
    {
        return GetFirstOuterActor()->GetWorld();
    }

    return nullptr;
}

void UAdvancedObject::Destroy()
{
    this->ConditionalBeginDestroy();
}
