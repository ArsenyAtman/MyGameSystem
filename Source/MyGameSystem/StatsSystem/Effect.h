// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "StatsSystemTypes.h"
#include "Effect.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEffectConditionChangeDelegate, class UEffect*, Effect);

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UEffect : public UAdvancedObject
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Effect|Internal")
	void Activate();

	UFUNCTION(BlueprintCallable, Category = "Effect|Control")
	void Deactivate();

	UFUNCTION(BlueprintPure, Category = "Effect|RelatedStatsComponent")
	class UStatsComponent* GetRelatedStatsComponent() const;

	UFUNCTION(BlueprintPure, Category = "Effect|RelatedActor")
	class AActor* GetRelatedActor() const;

	UPROPERTY(BlueprintAssignable, Category = "Effect|Delegates")
	FEffectConditionChangeDelegate OnEffectActivated;

	UPROPERTY(BlueprintAssignable, Category = "Effect|Delegates")
	FEffectConditionChangeDelegate OnEffectDeactivated;

protected:

	virtual void EndPlay_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Effect|Control", meta = (BlueprintProtected))
	void OnActivated();
	virtual void OnActivated_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Effect|Control", meta = (BlueprintProtected))
	void OnDeactivating();
	virtual void OnDeactivating_Implementation() { return; }

private:

	class UStatsComponent* RelatedStatsComponent;

	UFUNCTION(NetMulticast, Reliable)
	void Notify_OnActivated();

	void Broadcast_OnActivated();

	void Broadcast_OnDeactivated();
	
};
