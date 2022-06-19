// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

void UItem::Instance_Implementation(AItemInstance* ParentInstance)
{
    return;
}

void UItem::Uninstance_Implementation()
{
    return;
}

void UItem::SetItemInstance(AItemInstance* NewItemInstance)
{
    ItemInstance = NewItemInstance;
}
