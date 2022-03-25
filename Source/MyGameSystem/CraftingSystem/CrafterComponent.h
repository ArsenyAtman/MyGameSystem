// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrafterComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCraftConditionUpdateDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UCrafterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCrafterComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	bool CraftBegins(class UWorkbenchComponent* WithWorkbench);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Craft(class URecipeDataAsset* Recipe);

	UFUNCTION(BlueprintCallable)
	void CraftEnds();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void EndCraft();

	UFUNCTION(BlueprintPure)
	FORCEINLINE UWorkbenchComponent* GetWorkbench() { return Workbench; }

	UFUNCTION(BlueprintPure)
	TArray<class URecipeDataAsset*> GetRecipes(bool bCheckOnCompability); //was FORCEINLINE

	UFUNCTION(BlueprintPure)
	TArray<class UItem*> GetItems(); //was FORCEINLINE

	UPROPERTY(BlueprintAssignable)
	FCraftConditionUpdateDelegate OnCraftBeginned;

	UPROPERTY(BlueprintAssignable)
	FCraftConditionUpdateDelegate OnCraftEnded;

	UPROPERTY(BlueprintAssignable)
	FCraftConditionUpdateDelegate OnItemCrafted;

	UPROPERTY(BlueprintAssignable)
	FCraftConditionUpdateDelegate OnIncompatibleRecipe;

	UPROPERTY(BlueprintAssignable)
	FCraftConditionUpdateDelegate OnNotEnoughSpace;

	UPROPERTY(BlueprintAssignable)
	FCraftConditionUpdateDelegate OnNotEnoughResources;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void ItemCrafted();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void IncompatibleRecipe();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void NotEnoughSpace();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void NotEnoughResources();

protected:

	//

private:

	UPROPERTY(ReplicatedUsing = OnRep_Workbench)
	class UWorkbenchComponent* Workbench;

	UFUNCTION()
	void OnRep_Workbench();
};
