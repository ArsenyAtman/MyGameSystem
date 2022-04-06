// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatsSystemTypes.h"
#include "Effect.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FEffectInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEffectDataAsset* EffectData;

	FEffectInfo(class UEffectDataAsset* Data = nullptr)
	{
		EffectData = Data;
	}
};

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UEffect : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Effect|Internal")
	void Activate(class UStatsComponent* StatsComponent);

	UFUNCTION(BlueprintCallable, Category = "Effect|Internal")
	void Deactivate();

	UFUNCTION(BlueprintPure, Category = "Effect|RelatedStatsComponent")
	class UStatsComponent* GetRelatedStatsComponent() { return RelatedStatsComponent; }

	UFUNCTION(BlueprintPure, Category = "Effect|RelatedActor")
	class AActor* GetRelatedActor();

	UFUNCTION(BlueprintPure, Category = "Effect|Condition")
	FEffectInfo GetEffectInfo();

	UFUNCTION(BlueprintPure, Category = "Effect|Condition")
	bool GetIsActive() { return bIsActive; }

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Effect|Control")
	void OnActivated();
	virtual void OnActivated_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Effect|Control")
	void OnDeactivating();
	virtual void OnDeactivating_Implementation() { return; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect|Condition")
	class UEffectDataAsset* EffectData = nullptr;

private:

	class UStatsComponent* RelatedStatsComponent;

	bool bIsActive = false;
	
};
