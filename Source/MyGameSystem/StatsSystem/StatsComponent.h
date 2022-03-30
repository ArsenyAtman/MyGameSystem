// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsSystemTypes.h"
#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStatsComponentUpdateDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatsComponentEffectDelegate, class UEffect*, Effect);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatsComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffect(class UEffect* Effect);

	UFUNCTION(BlueprintCallable)
	void AbortEffect(class UEffect* Effect);

	UFUNCTION(BlueprintCallable)
	void AddEffect(class UEffect* Effect);

	UFUNCTION(BlueprintCallable)
	void RemoveEffect(class UEffect* Effect);

	UFUNCTION(BlueprintGetter)
	TArray<class UEffect*> GetEffects() { return Effects; }

	UFUNCTION(BlueprintGetter)
	TArray<class UStat*> GetStats() { return Stats; }

	UFUNCTION(BlueprintPure)
	TArray<class UStat*> GetStatsOfClass(TSubclassOf<class UStat> StatClass);

	UFUNCTION(BlueprintGetter)
	TArray<struct FEffectInfo> GetEffectsInfo() { return EffectsInfo; }

	UFUNCTION(BlueprintGetter)
	TArray<struct FStatInfo> GetStatsInfo() { return StatsInfo; }

	UPROPERTY(BlueprintAssignable)
	FStatsComponentUpdateDelegate OnEffectsUpdated;

	UPROPERTY(BlueprintAssignable)
	FStatsComponentUpdateDelegate OnStatsUpdated;

	UPROPERTY(BlueprintAssignable)
	FStatsComponentEffectDelegate OnEffectAdded;

	UPROPERTY(BlueprintAssignable)
	FStatsComponentEffectDelegate OnEffectRemoved;

protected:

	UPROPERTY(EditAnywhere, Instanced, BlueprintGetter = GetStats)
	TArray<class UStat*> Stats;

	UPROPERTY(BlueprintGetter = GetEffects)
	TArray<class UEffect*> Effects;

	UPROPERTY(BlueprintGetter = GetEffectsInfo, ReplicatedUsing = OnRep_EffectsInfo)
	TArray<struct FEffectInfo> EffectsInfo;

	UPROPERTY(BlueprintGetter = GetStatsInfo, ReplicatedUsing = OnRep_StatsInfo)
	TArray<struct FStatInfo> StatsInfo;

private:

	UFUNCTION()
	void OnRep_StatsInfo();

	UFUNCTION()
	void StatValuesChanged(FStatValues Delta, class UEffect* OfEffect) { UpdateStatsInfo(); }

	UFUNCTION()
	void StatEffectAdded(class UEffect* Effect) { UpdateStatsInfo(); }

	UFUNCTION()
	void StatEffectRemoved(class UEffect* Effect) { UpdateStatsInfo(); }

	UFUNCTION()
	void UpdateStatsInfo();

	UFUNCTION()
	void OnRep_EffectsInfo();

	UFUNCTION()
	void UpdateEffectsInfo();

};
