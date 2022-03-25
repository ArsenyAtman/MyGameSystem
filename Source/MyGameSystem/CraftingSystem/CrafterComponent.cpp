// Fill out your copyright notice in the Description page of Project Settings.


#include "CrafterComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WorkbenchComponent.h"
#include "RecipeDataAsset.h"
#include "CraftActorInterface.h"
#include "RecipeBookComponent.h"
#include "MyGameSystem/InventorySystem/InventoryComponent.h"

// Sets default values for this component's properties
UCrafterComponent::UCrafterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCrafterComponent::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UCrafterComponent, Workbench, COND_OwnerOnly);
}

bool UCrafterComponent::CraftBegins(UWorkbenchComponent* WithWorkbench)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (!IsValid(Workbench) && IsValid(WithWorkbench))
		{
			Workbench = WithWorkbench;
			OnCraftBeginned.Broadcast();
			return true;
		}
	}
	return false;
}

bool UCrafterComponent::Craft_Validate(URecipeDataAsset* Recipe)
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "Recipe Validation!");
	/*if (Recipe->StaticClass()->IsChildOf(ForRecipesOfType) && bIsCrafting)
	{
		if (IsValid(ComponentsForCrafting.PlayerRecipeBook))
		{
			if (ComponentsForCrafting.PlayerRecipeBook->GetRecipes().Find(Recipe) != INDEX_NONE)
			{
				return true;
			}
		}
		if (IsValid(ComponentsForCrafting.CraftRecipeBook))
		{
			if (ComponentsForCrafting.CraftRecipeBook->GetRecipes().Find(Recipe) != INDEX_NONE)
			{
				return true;
			}
		}
	}
	return false;
	*/
	return true;
}

void UCrafterComponent::Craft_Implementation(URecipeDataAsset* Recipe)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Workbench))
		{
			Workbench->Craft(Recipe);
		}
	}
}

void UCrafterComponent::CraftEnds()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		Workbench = nullptr;
		OnCraftEnded.Broadcast();
	}
}

void UCrafterComponent::EndCraft_Implementation()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Workbench))
		{
			Workbench->EndCrafting();
		}
	}
}

TArray<URecipeDataAsset*> UCrafterComponent::GetRecipes(bool bCheckOnCompability)
{
	TArray<URecipeDataAsset*> Recipes;

	if (IsValid(Workbench))
	{
		if (IsValid(this->GetOwner()))
		{
			if (this->GetOwner()->Implements<UCraftActorInterface>())
			{
				URecipeBookComponent* RecipeBook = ICraftActorInterface::Execute_GetRecipeBookComponent(this->GetOwner());
				if (IsValid(RecipeBook))
				{
					if (bCheckOnCompability)
					{
						for (URecipeDataAsset* Recipe : RecipeBook->GetRecipes())
						{
							if (Workbench->CheckRecipeOnCompability(Recipe))
							{
								Recipes.Add(Recipe);
							}
						}
					}
					else
					{
						Recipes.Append(RecipeBook->GetRecipes());
					}
				}
			}
		}


		if (IsValid(Workbench->GetOwner()) && IsValid(this->GetOwner()))
		{
			if (Workbench->GetOwner() != this->GetOwner())
			{
				if (Workbench->GetOwner()->Implements<UCraftActorInterface>())
				{
					URecipeBookComponent* RecipeBook = ICraftActorInterface::Execute_GetRecipeBookComponent(Workbench->GetOwner());
					if (IsValid(RecipeBook))
					{
						if (bCheckOnCompability)
						{
							for (URecipeDataAsset* Recipe : RecipeBook->GetRecipes())
							{
								if (Workbench->CheckRecipeOnCompability(Recipe))
								{
									Recipes.Add(Recipe);
								}
							}
						}
						else
						{
							Recipes.Append(RecipeBook->GetRecipes());
						}
					}
				}
			}
		}
	}

	return Recipes;
}

TArray<UItem*> UCrafterComponent::GetItems()
{
	TArray<UItem*> Items;

	if (IsValid(this->GetOwner()))
	{
		if (this->GetOwner()->Implements<UCraftActorInterface>())
		{
			UInventoryComponent* Inventory = ICraftActorInterface::Execute_GetCraftInventoryComponent(this->GetOwner());
			if (IsValid(Inventory))
			{
				for (FItemStruct& Item : IItemContainerInterface::Execute_GetItems(Inventory))
				{
					if (IsValid(Item.Item))
					{
						Items.Add(Item.Item);
					}
				}
			}
		}
	}

	if (IsValid(Workbench))
	{
		if (IsValid(Workbench->GetOwner()) && IsValid(this->GetOwner()))
		{
			if (Workbench->GetOwner() != this->GetOwner())
			{
				if (Workbench->GetOwner()->Implements<UCraftActorInterface>())
				{
					UInventoryComponent* Inventory = ICraftActorInterface::Execute_GetCraftInventoryComponent(Workbench->GetOwner());
					if (IsValid(Inventory))
					{
						for (FItemStruct& Item : IItemContainerInterface::Execute_GetItems(Inventory))
						{
							if (IsValid(Item.Item))
							{
								Items.Add(Item.Item);
							}
						}
					}
				}
			}
		}
	}

	return Items;
}

void UCrafterComponent::OnRep_Workbench()
{
	if (IsValid(Workbench))
	{
		OnCraftBeginned.Broadcast();
	}
	else
	{
		OnCraftEnded.Broadcast();
	}
}

void UCrafterComponent::ItemCrafted_Implementation()
{
	OnItemCrafted.Broadcast();
}

void UCrafterComponent::IncompatibleRecipe_Implementation()
{
	OnIncompatibleRecipe.Broadcast();
}

void UCrafterComponent::NotEnoughSpace_Implementation()
{
	OnNotEnoughSpace.Broadcast();
}

void UCrafterComponent::NotEnoughResources_Implementation()
{
	OnNotEnoughResources.Broadcast();
}