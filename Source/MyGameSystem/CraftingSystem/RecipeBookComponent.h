// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RecipeBookComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRecipeBookConditionUpdateDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API URecipeBookComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URecipeBookComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	bool CheckRecipe(class URecipeDataAsset* Recipe);

	UFUNCTION(BlueprintCallable)
	void AddRecipe(class URecipeDataAsset* Recipe);

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<class URecipeDataAsset*> GetRecipes() { return Recipes; }

	UPROPERTY(BlueprintAssignable)
	FRecipeBookConditionUpdateDelegate OnRecipesUpdated;

protected:

	//

private:	
	
	UPROPERTY(ReplicatedUsing = OnRep_Recipes, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<class URecipeDataAsset*> Recipes;
	
	UFUNCTION()
	void OnRep_Recipes();
};
