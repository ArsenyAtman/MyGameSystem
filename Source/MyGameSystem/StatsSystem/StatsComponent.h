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

	UFUNCTION(BlueprintCallable, Category = "StatComponent|Control")
	void ApplyEffect(class UEffect* Effect);

	UFUNCTION(BlueprintCallable, Category = "StatComponent|Control")
	void AbortEffect(class UEffect* Effect);

	UFUNCTION(BlueprintCallable, Category = "StatComponent|Internal")
	void AddEffect(class UEffect* Effect);

	UFUNCTION(BlueprintCallable, Category = "StatComponent|Internal")
	void RemoveEffect(class UEffect* Effect);

	UFUNCTION(BlueprintGetter, Category = "StatComponent|Effects")
	TArray<class UEffect*> GetEffects() const { return Effects; }

	UFUNCTION(BlueprintGetter, Category = "StatComponent|Stats")
	TArray<class UStat*> GetStats() const { return Stats; }

	UFUNCTION(BlueprintPure, Category = "StatComponent|Stats")
	TArray<class UStat*> GetStatsOfClass(TSubclassOf<class UStat> StatClass) const;

	UFUNCTION(BlueprintGetter, Category = "StatComponent|Effects")
	TArray<struct FEffectInfo> GetEffectsInfo() const { return EffectsInfo; }

	UFUNCTION(BlueprintGetter, Category = "StatComponent|Stats")
	TArray<struct FStatInfo> GetStatsInfo() const { return StatsInfo; }

	UPROPERTY(BlueprintAssignable, Category = "StatComponent|Delegates")
	FStatsComponentUpdateDelegate OnEffectsUpdated;

	UPROPERTY(BlueprintAssignable, Category = "StatComponent|Delegates")
	FStatsComponentUpdateDelegate OnStatsUpdated;

	UPROPERTY(BlueprintAssignable, Category = "StatComponent|Delegates")
	FStatsComponentEffectDelegate OnEffectAdded;

	UPROPERTY(BlueprintAssignable, Category = "StatComponent|Delegates")
	FStatsComponentEffectDelegate OnEffectRemoved;

protected:

	UPROPERTY(EditAnywhere, Instanced, BlueprintGetter = GetStats, Category = "StatComponent|Stats")
	TArray<class UStat*> Stats;

	UPROPERTY(BlueprintGetter = GetEffects, Category = "StatComponent|Effects")
	TArray<class UEffect*> Effects;

	UPROPERTY(BlueprintGetter = GetEffectsInfo, ReplicatedUsing = OnRep_EffectsInfo, Category = "StatComponent|Effects")
	TArray<struct FEffectInfo> EffectsInfo;

	UPROPERTY(BlueprintGetter = GetStatsInfo, ReplicatedUsing = OnRep_StatsInfo, Category = "StatComponent|Stats")
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
