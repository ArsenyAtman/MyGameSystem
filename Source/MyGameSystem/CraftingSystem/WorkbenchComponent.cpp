// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkbenchComponent.h"
#include "RecipeDataAsset.h"
#include "RecipeBookComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RecipeType.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "MyGameSystem/InventorySystem/InventoryComponent.h"
#include "MyGameSystem/InventorySystem/Item.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "CrafterComponent.h"
#include "CraftActorInterface.h"

// Sets default values for this component's properties
UWorkbenchComponent::UWorkbenchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UWorkbenchComponent::BeginCrafting(UCrafterComponent* WithCrafter)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (!IsValid(Crafter) && IsValid(WithCrafter))
		{
			if (WithCrafter->CraftBegins(this))
			{
				Crafter = WithCrafter;
			}
		}
	}
}

void UWorkbenchComponent::Craft(URecipeDataAsset* Recipe)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Recipe) && IsValid(Crafter))
		{
			if (CheckRecipeOnCompability(Recipe))
			{
				UInventoryComponent* CrafterInventory = nullptr;
				UInventoryComponent* OwnInventory = nullptr;
				URecipeBookComponent* CrafterRecipeBook = nullptr;
				URecipeBookComponent* OwnRecipeBook = nullptr;

				AActor* CrafterActor = Crafter->GetOwner();
				AActor* OwnActor = this->GetOwner();

				if (CrafterActor->Implements<UCraftActorInterface>())
				{
					CrafterInventory = ICraftActorInterface::Execute_GetCraftInventoryComponent(CrafterActor);
					CrafterRecipeBook = ICraftActorInterface::Execute_GetRecipeBookComponent(CrafterActor);
				}

				if (OwnActor->Implements<UCraftActorInterface>() && CrafterActor != OwnActor)
				{
					OwnInventory = ICraftActorInterface::Execute_GetCraftInventoryComponent(OwnActor);
					OwnRecipeBook = ICraftActorInterface::Execute_GetRecipeBookComponent(OwnActor);
				}

				if (IsRecipeInBooks(Recipe, {CrafterRecipeBook, OwnRecipeBook}))
				{
					int DeltaCount = 0;

					for (FCraftItemStruct ItemForCraft : Recipe->RequiredItems)
					{
						DeltaCount -= ItemForCraft.Count;
					}
					for (FCraftItemStruct ItemForCraft : Recipe->CraftedItems)
					{
						DeltaCount += ItemForCraft.Count;
					}
					if (IsValid(CrafterInventory))
					{
						DeltaCount -= CrafterInventory->GetInventorySize() - CrafterInventory->GetItemsCount();
					}
					if (IsValid(OwnInventory))
					{
						DeltaCount -= OwnInventory->GetInventorySize() - OwnInventory->GetItemsCount();
					}

					if (DeltaCount > 0)
					{
						Crafter->NotEnoughSpace();
						return;
					}

					for (FCraftItemStruct ItemForCraft : Recipe->RequiredItems)
					{
						//UKismetSystemLibrary::PrintString(GetWorld(), FString::FromInt(GetCountOfItemsInInventory(CrafterInventory, ItemForCraft.Item) + GetCountOfItemsInInventory(OwnInventory, ItemForCraft.Item)), true, true, FLinearColor::Blue);
						if (GetCountOfItemsInInventory(CrafterInventory, ItemForCraft.Item) + GetCountOfItemsInInventory(OwnInventory, ItemForCraft.Item) < ItemForCraft.Count)
						{
							Crafter->NotEnoughResources();
							return;
						}
					}

					for (FCraftItemStruct ItemForCraft : Recipe->RequiredItems)
					{
						int CountOfTakenItems = ExtractItemsFromInventory(CrafterInventory, ItemForCraft.Item, ItemForCraft.Count);
						ExtractItemsFromInventory(OwnInventory, ItemForCraft.Item, ItemForCraft.Count - CountOfTakenItems);
					}

					for (FCraftItemStruct ItemForCraft : Recipe->CraftedItems)
					{
						int CountOfItemsToPlaceLeft = ItemForCraft.Count;
						CountOfItemsToPlaceLeft -= AddItemsToInventory(CrafterInventory, ItemForCraft.Item, CountOfItemsToPlaceLeft);
						CountOfItemsToPlaceLeft -= AddItemsToInventory(OwnInventory, ItemForCraft.Item, CountOfItemsToPlaceLeft);
					}

					Crafter->ItemCrafted();
				}
			}
			else
			{
				Crafter->IncompatibleRecipe();
			}
		}
	}
}

void UWorkbenchComponent::EndCrafting()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		Crafter->CraftEnds();
		Crafter = nullptr;
	}
}

bool UWorkbenchComponent::CheckRecipeOnCompability(class URecipeDataAsset* Recipe)
{
	return Recipe->RecipeType->IsChildOf(ForRecipesOfType);
}

int UWorkbenchComponent::GetCountOfItemsInInventory(UInventoryComponent* Inventory, UItem* Item) const
{
	int Count = 0;
	if (IsValid(Inventory) && IsValid(Item))
	{
		for (FItemStruct ItemStruct : IItemContainerInterface::Execute_GetItems(Inventory))
		{
			if (ItemStruct.Item == Item)
			{
				Count++;
			}
		}
	}
	return Count;
}

int UWorkbenchComponent::ExtractItemsFromInventory(UInventoryComponent* Inventory, UItem* Item, int Count)
{
	int ExtractedCount = 0;
	if (IsValid(Inventory) && IsValid(Item))
	{
		for (; ExtractedCount < Count; ExtractedCount++)
		{
			if (!Inventory->RemoveItemOfClass(Item))
			{
				//UKismetSystemLibrary::PrintString(GetWorld(), "EXT");
				break;
			}
		}
	}
	return ExtractedCount;
}

int UWorkbenchComponent::AddItemsToInventory(UInventoryComponent* Inventory, UItem* Item, int Count)
{
	int AddedCount = 0;
	if (IsValid(Inventory) && IsValid(Item))
	{
		for (; AddedCount < Count; AddedCount++)
		{
			if (!IItemContainerInterface::Execute_AddItem(Inventory, FItemStruct(Item), -1))
			{
				//UKismetSystemLibrary::PrintString(GetWorld(), "ADD");
				break;
			}
		}
	}
	return AddedCount;
}

bool UWorkbenchComponent::IsRecipeInBooks(URecipeDataAsset* Recipe, const TArray<URecipeBookComponent*>& RecipeBooks)
{
	for (URecipeBookComponent* Book : RecipeBooks)
	{
		if (IsValid(Book))
		{
			if (Book->GetRecipes().Find(Recipe) != INDEX_NONE)
			{
				return true;
			}
		}
	}
	return false;
}
