// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "InventoryComponent.h"
#include "ComplexItem.h"
#include "ItemPlace.h"
#include "ItemResizer.h"
#include "Net/UnrealNetwork.h"

void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AItem, ItemPossession);
}

void AItem::Instance_Implementation()
{
    if(HasAuthority() == false)
    {
        return;
    }

    ShowView();
    InstanceItem();
}

void AItem::Uninstance_Implementation()
{
    if(HasAuthority() == false)
    {
        return;
    }

    HideView();
    UninstanceItem();
}

FVector2D AItem::GetInventorySize_Implementation() const
{
    if (IsValid(ItemResizer))
    {
        return ItemResizer->GetInventorySize(this, GetPossessingPlace());
    }

    return FVector2D::ZeroVector;
}

FVector2D AItem::GetInventorySizeForPlace_Implementation(UItemPlace* Place) const
{
    if (IsValid(ItemResizer))
    {
        return ItemResizer->GetInventorySize(this, Place);
    }

    return FVector2D::ZeroVector;
}

UInventoryComponent* AItem::GetRelatedInventory() const
{
    if (IsValid(GetPossessingPlace()))
    {
        UObject* Possessor = GetPossessingPlace()->GetPossessor();

        while (Possessor)
        {
            UInventoryComponent* Inventory = Cast<UInventoryComponent>(Possessor);
            if (IsValid(Inventory))
            {
                return Inventory;
            }
            else
            {
                AItem* Item = Cast<AItem>(Possessor);
                if (IsValid(Item) && IsValid(Item->GetPossessingPlace()))
                {
                    Possessor = Item->GetPossessingPlace()->GetPossessor();
                }
            }
        }
    }

    return nullptr;
}

void AItem::PlaceInPlace(UItemPlace* NewPlace, FVector2D NewLocation)
{
    if(HasAuthority() == false)
    {
        return;
    }

    if (IsValid(GetPossessingPlace()))
    {
        GetPossessingPlace()->RemoveItem(this);
    }
    SetPossession(FItemPossessionInfo(NewLocation, NewPlace));
    Placed(NewPlace);
}

void AItem::RemoveFromPlace()
{
    if(HasAuthority() == false)
    {
        return;
    }

    UItemPlace* PrevPlace = GetPossessingPlace();
    SetPossession(FItemPossessionInfo(FVector2D::ZeroVector, nullptr));
    Removed(PrevPlace);
}

void AItem::SetPossession(FItemPossessionInfo NewPossessionInfo)
{
    if(HasAuthority() == false)
    {
        return;
    }

    FItemPossessionInfo PrevPossessionInfo = ItemPossession;
    ItemPossession = NewPossessionInfo;
    Broadcast_PossessionChanged(PrevPossessionInfo);
}

void AItem::OnRep_ItemPossession(FItemPossessionInfo PrevPossession)
{
    Broadcast_PossessionChanged(PrevPossession);
}

void AItem::Broadcast_PossessionChanged(FItemPossessionInfo PrevPossession)
{
    OnPossessionChanged.Broadcast(this, ItemPossession, PrevPossession);
}

void AItem::Broadcast_ItemInstanced()
{
    OnInstanced.Broadcast(this);
}

void AItem::Broadcast_ItemUninstanced()
{
    OnUninstanced.Broadcast(this);
}

void AItem::InstanceItem_Implementation()
{
    UItemPlace* OwningPlace = GetPossessingPlace();
    if (IsValid(OwningPlace))
    {
        AttachToComponent(OwningPlace, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        SetActorRelativeTransform(OwningPlace->GetRelativeTransformForItem(this));
    }

    Broadcast_ItemInstanced();
}

void AItem::UninstanceItem_Implementation()
{
    if (IsValid(GetPossessingPlace()))
    {
        DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }

    Broadcast_ItemUninstanced();
}
