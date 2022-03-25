// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeBookComponent.h"
#include "RecipeDataAsset.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
URecipeBookComponent::URecipeBookComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void URecipeBookComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(URecipeBookComponent, Recipes, COND_OwnerOnly);
}

bool URecipeBookComponent::CheckRecipe(URecipeDataAsset* Recipe)
{
	return Recipes.Find(Recipe) != INDEX_NONE;
}

void URecipeBookComponent::AddRecipe(URecipeDataAsset* Recipe)
{
	Recipes.Add(Recipe);
}

void URecipeBookComponent::OnRep_Recipes()
{
	OnRecipesUpdated.Broadcast();
}