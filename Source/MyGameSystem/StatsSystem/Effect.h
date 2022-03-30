// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatsSystemTypes.h"
#include "Engine/DataAsset.h"
#include "Effect.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UEffectDataAsset : public UDataAsset
{
	GENERATED_BODY()

};

USTRUCT(BlueprintType, Blueprintable)
struct FEffectInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UEffect> EffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEffectDataAsset* EffectData;

	FEffectInfo(TSubclassOf<class UEffect> Class = nullptr, class UEffectDataAsset* Data = nullptr)
	{
		EffectClass = Class;
		EffectData = Data;
	}
};

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
	class UEffectDataAsset* EffectData = nullptr;

private:

	class UStatsComponent* RelatedStatsComponent;

	bool bIsActive = false;
	
};
