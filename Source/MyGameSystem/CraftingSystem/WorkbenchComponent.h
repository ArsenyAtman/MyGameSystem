// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorkbenchComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UWorkbenchComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWorkbenchComponent();

	UFUNCTION(BlueprintCallable)
	void BeginCrafting(class UCrafterComponent* WithCrafter);

	UFUNCTION(BlueprintCallable)
	void Craft(class URecipeDataAsset* Recipe);

	UFUNCTION(BlueprintCallable)
	void EndCrafting();

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UCrafterComponent* GetCrafter() { return Crafter; }

	UFUNCTION(BlueprintPure)
	bool CheckRecipeOnCompability(class URecipeDataAsset* Recipe); //was FORCEINLINE

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class URecipeType> ForRecipesOfType;

	UFUNCTION(BlueprintPure)
	int GetCountOfItemsInInventory(class UInventoryComponent* Inventory, class UItem* Item) const;

	UFUNCTION(BlueprintCallable)
	int ExtractItemsFromInventory(class UInventoryComponent* Inventory, class UItem* Item, int Count);

	UFUNCTION(BlueprintCallable)
	int AddItemsToInventory(class UInventoryComponent* Inventory, class UItem* Item, int Count);

	UFUNCTION(BlueprintPure)
	bool IsRecipeInBooks(class URecipeDataAsset* Recipe, const TArray<class URecipeBookComponent*>& RecipeBooks);

private:

	class UCrafterComponent* Crafter;
		
};
