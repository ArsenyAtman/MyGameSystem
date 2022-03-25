// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WalletComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWalletCondiitonUpdateDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UWalletComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWalletComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	bool Transaction(class UWalletComponent* ToWallet, float Amount);

	UFUNCTION(BlueprintCallable)
	bool AddCash(float Amount);

	UFUNCTION(BlueprintPure)
	float GetCash() { return Cash; }

	UPROPERTY(BlueprintAssignable)
	FWalletCondiitonUpdateDelegate OnCashUpdated;

private:	
	
	UPROPERTY(ReplicatedUsing = OnRep_Cash, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Cash;
		
	UFUNCTION()
	void OnRep_Cash();
};
