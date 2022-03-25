// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Engine/Texture2D.h"

bool FItemStruct::operator == (const FItemStruct& OtherItem) const
{
	return this->Item == OtherItem.Item;//&& this->OwningContainer == OtherItem.OwningContainer && this->Place == OtherItem.Place;
}