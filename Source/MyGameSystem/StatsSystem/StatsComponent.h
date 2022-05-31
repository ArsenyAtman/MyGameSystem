// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/ReplicatingActorComponent.h"
#include "StatsSystemTypes.h"
#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatComponentEffectDelegate, class UEffect*, Effect);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UStatsComponent : public UReplicatingActorComponent
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

	UFUNCTION(BlueprintPure, Category = "StatComponent|Effects")
	TArray<class UEffect*> GetEffectsOfClass(TSubclassOf<class UEffect> EffectClass) const;

	UFUNCTION(BlueprintGetter, Category = "StatComponent|Stats")
	TArray<class UStat*> GetStats() const { return Stats; }

	UFUNCTION(BlueprintPure, Category = "StatComponent|Stats")
	TArray<class UStat*> GetStatsOfClass(TSubclassOf<class UStat> StatClass) const;

	UPROPERTY(BlueprintAssignable)
	FStatComponentEffectDelegate OnEffectAdded;

	UPROPERTY(BlueprintAssignable)
	FStatComponentEffectDelegate OnEffectRemoved;

protected:

	// ...

private:

	UPROPERTY(EditAnywhere, Instanced, BlueprintGetter = GetStats, Replicated, Category = "StatComponent|Stats")
	TArray<class UStat*> Stats;

	UPROPERTY(BlueprintGetter = GetEffects, ReplicatedUsing = OnRep_Effects, Category = "StatComponent|Effects")
	TArray<class UEffect*> Effects;

	UFUNCTION()
	void OnRep_Effects(const TArray<class UEffect*>& PreReplicationEffects);

	UFUNCTION()
	void BroadcastChange_Effects(const TArray<class UEffect*>& PrevEffects);

	UFUNCTION()
	void Broadcast_OnEffectAdded(class UEffect* Effect);

	UFUNCTION()
	void Broadcast_OnEffectRemoved(class UEffect* Effect);

	TArray<class UEffect*> FindMissingEffects(const TArray<class UEffect*>& FromArray, const TArray<class UEffect*>& InArray) const;

};
