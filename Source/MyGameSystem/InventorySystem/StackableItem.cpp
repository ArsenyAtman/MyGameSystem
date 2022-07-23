// Fill out your copyright notice in the Description page of Project Settings.


#include "StackableItem.h"

bool AStackableItem::AddItem_Implementation(AItem* Item)
{
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
    CountInStack = InitCountInStack;
}

void AStackableItem::MergedWithItem(int32 CountOfTakenItems)
{
    CountInStack -= CountOfTakenItems;

    if(CountInStack <= 0)
    {
        this->RemoveFromPlace();
        this->Destroy();
    }
}

int32 AStackableItem::MergeWithItem(AStackableItem* Item)
{
    int32 IncomingCount = Item->GetCountInStack();
    int32 CurrentCount = this->GetCountInStack();
    int32 MaxCount = this->GetMaxCountInStack();

    int32 CountOfAddedItems = FMath::Clamp(IncomingCount, 0, MaxCount - CurrentCount);

    CountInStack += CountOfAddedItems;
    Item->MergedWithItem(CountOfAddedItems);

    int32 CountOfItemsLeft = IncomingCount - CountOfAddedItems;
    return CountOfItemsLeft;
}

AStackableItem* AStackableItem::Split(int32 CountToTake)
{
    if (CountToTake >= this->GetCountInStack())
    {
        return this;
    }

    CountInStack -= CountToTake;

    AStackableItem* NewItem = GetWorld()->SpawnActorDeferred<AStackableItem>(this->StaticClass(), FTransform::Identity, this->GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    NewItem->InitializeCountInStack(CountToTake);
    NewItem->FinishSpawning(FTransform::Identity);
    return NewItem;
}
