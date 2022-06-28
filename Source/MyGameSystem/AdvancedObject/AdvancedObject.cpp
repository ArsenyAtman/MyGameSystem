// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedObject.h"
#include "Kismet/KismetSystemLibrary.h"

void UAdvancedObject::PostInitProperties()
{
    Super::PostInitProperties();

    if(IsValid(GetWorld()))
    {
        SetupTick();
        BeginPlay();
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
    MulticastBeginDestroy();
}

void UAdvancedObject::ChangeOuter(UObject* NewOuter)
{
    Rename(nullptr, NewOuter, REN_None);
}

UObject* UAdvancedObject::GetDefaultObject()
{
    return StaticClass()->GetDefaultObject();
}

void UAdvancedObject::MulticastBeginDestroy_Implementation()
{
    RemoveTick();
    EndPlay();
    this->ConditionalBeginDestroy();
}

void UAdvancedObject::SetupTick()
{
    if(bCanEverTick)
    {
        TickDelegate = FTickerDelegate::CreateUObject(this, &UAdvancedObject::TickerTick);
        TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate);
    }
}

void UAdvancedObject::RemoveTick()
{
    FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
}

bool UAdvancedObject::TickerTick(float DeltaTime)
{
    ObjectTick(DeltaTime);
    return true;
}
