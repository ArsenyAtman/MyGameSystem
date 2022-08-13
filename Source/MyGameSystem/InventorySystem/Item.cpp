// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "InventoryComponent.h"
#include "ComplexItem.h"
#include "ItemPlace.h"
#include "ItemResizer.h"
#include "Net/UnrealNetwork.h"

AItem::AItem()
{
    bReplicates = true;
    SetReplicateMovement(true);
}

void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AItem, ItemPossession);
    DOREPLIFETIME(AItem, bIsInstanced);
}

void AItem::SetIsInstanced_Implementation(bool bNewIsInstanced)
{
    if(HasAuthority() == false)
    {
        return;
    }

    bIsInstanced = bNewIsInstanced;

    UItemPlace* OwningPlace = GetPossessingPlace();
    if(GetIsInstanced() && IsValid(OwningPlace))
    {
        AttachToComponent(OwningPlace, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        SetActorRelativeTransform(OwningPlace->GetRelativeTransformForItem(this));
    }
    else
    {
        DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }

    IsInstancedChanged();
    Broadcast_ItemInstancedChanged();
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

void AItem::RemoveFromCurrentPlace()
{
    if (HasAuthority() && IsValid(GetPossessingPlace()))
    {
        GetPossessingPlace()->RemoveItem(this);
    }
}

void AItem::ChangePossession(FItemPossessionInfo NewPossessionInfo)
{
    if(HasAuthority() == false)
    {
        return;
    }

    FItemPossessionInfo PrevPossessionInfo = ItemPossession;
    ItemPossession = NewPossessionInfo;

    PossessionChanged(ItemPossession, PrevPossessionInfo);
    Broadcast_PossessionChanged(PrevPossessionInfo);
}

void AItem::OnRep_ItemPossession(FItemPossessionInfo PrevPossession)
{
    PossessionChanged(ItemPossession, PrevPossession);
    Broadcast_PossessionChanged(PrevPossession);
}

void AItem::Broadcast_PossessionChanged(FItemPossessionInfo PrevPossession)
{
    OnPossessionChanged.Broadcast(this, ItemPossession, PrevPossession);
}

void AItem::OnRep_IsInstanced(bool PrevIsInstanced)
{
    IsInstancedChanged();
    Broadcast_ItemInstancedChanged();
}

void AItem::Broadcast_ItemInstancedChanged()
{
    OnInstancedChanged.Broadcast(this);
}
