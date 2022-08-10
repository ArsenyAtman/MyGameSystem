// Fill out your copyright notice in the Description page of Project Settings.


#include "StackableItem.h"

#include "Net/UnrealNetwork.h"

void AStackableItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AStackableItem, CountInStack);
}

bool AStackableItem::AddItem_Implementation(AItem* Item)
{
    if(HasAuthority() == false)
    {
        return false;
    }

    AStackableItem* StackableItem = Cast<AStackableItem>(Item);
    int32 CountOfItemsBeforeMerge = StackableItem->GetCountInStack();
    if (IsValid(StackableItem) && StackableItem->StaticClass() == this->StaticClass())
    {
        int32 CountOfItemsLeft = MergeWithItem(StackableItem);
        if (CountOfItemsLeft == 0)
        {
            return true;
        }
    }

    return false;
}

void AStackableItem::InitializeCountInStack(int32 InitCountInStack)
{
    if(HasAuthority() == false)
    {
        return;
    }

    if(IsActorInitialized())
    {
        return;
    }

    SetCountInStack(InitCountInStack);
}

void AStackableItem::MergedWithItem(int32 CountOfTakenItems)
{
    if(HasAuthority() == false)
    {
        return;
    }

    SetCountInStack(GetCountInStack() - CountOfTakenItems);

    if(CountInStack <= 0)
    {
        this->RemoveFromPlace();
        this->Destroy();
    }
}

int32 AStackableItem::MergeWithItem(AStackableItem* Item)
{
    if(HasAuthority() == false)
    {
        return false;
    }

    int32 IncomingCount = Item->GetCountInStack();
    int32 CurrentCount = this->GetCountInStack();
    int32 MaxCount = this->GetMaxCountInStack();

    int32 CountOfAddedItems = FMath::Clamp(IncomingCount, 0, MaxCount - CurrentCount);

    SetCountInStack(GetCountInStack() + CountOfAddedItems);

    Item->MergedWithItem(CountOfAddedItems);

    int32 CountOfItemsLeft = IncomingCount - CountOfAddedItems;
    return CountOfItemsLeft;
}

AStackableItem* AStackableItem::Split(int32 CountToTake)
{
    if(HasAuthority() == false)
    {
        return nullptr;
    }

    if (CountToTake >= this->GetCountInStack())
    {
        return this;
    }

    SetCountInStack(GetCountInStack() - CountToTake);

    AStackableItem* NewItem = GetWorld()->SpawnActorDeferred<AStackableItem>(this->StaticClass(), FTransform::Identity, this->GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    NewItem->InitializeCountInStack(CountToTake);
    NewItem->FinishSpawning(FTransform::Identity);
    return NewItem;
}

void AStackableItem::SetCountInStack(int32 NewCountInStack)
{
    if(HasAuthority() == false)
    {
        return;
    }

    int32 PrevCountInStack = GetCountInStack();
    CountInStack = NewCountInStack;
    CountInStackChanged(GetCountInStack(), PrevCountInStack);
    Broadcast_CountInStackChanged(PrevCountInStack);
}

void AStackableItem::OnRep_CountInStack(int32 PrevCountInStack)
{
    Broadcast_CountInStackChanged(PrevCountInStack);
}

void AStackableItem::Broadcast_CountInStackChanged(int32 PrevCountInStack)
{
    CountInStackChanged(GetCountInStack(), PrevCountInStack);
    OnCountInStackChanged.Broadcast(this, GetCountInStack(), PrevCountInStack);
}
