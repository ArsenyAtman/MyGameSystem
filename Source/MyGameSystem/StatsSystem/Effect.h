// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatsSystemTypes.h"
#include "Effect.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UEffect : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void Activate(class UStatsComponent* StatsComponent);

	UFUNCTION(BlueprintCallable)
	void Deactivate();

	UFUNCTION(BlueprintPure)
	class UStatsComponent* GetRelatedStatsComponent() { return RelatedStatsComponent; }

	UFUNCTION(BlueprintPure)
	class AActor* GetRelatedActor();

	UFUNCTION(BlueprintPure)
	FEffectInfo GetEffectInfo();

	UFUNCTION(BlueprintPure)
	bool GetIsActive() { return bIsActive; }

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnActivated();
	virtual void OnActivated_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDeactivating();
	virtual void OnDeactivating_Implementation() { return; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText EffectName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText EffectDescription;

private:

	class UStatsComponent* RelatedStatsComponent;

	bool bIsActive = false;
	
};
