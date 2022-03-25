// Fill out your copyright notice in the Description page of Project Settings.


#include "WalletComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UWalletComponent::UWalletComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UWalletComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWalletComponent, Cash);
}

bool UWalletComponent::Transaction(UWalletComponent* ToWallet, float Amount)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (Amount <= Cash)
		{
			Cash -= Amount;
			bool bResult = ToWallet->AddCash(Amount);
			OnCashUpdated.Broadcast();
			return bResult;
		}
	}
	return false;
}

bool UWalletComponent::AddCash(float Amount)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		Cash += Amount;
		OnCashUpdated.Broadcast();
		return true;
	}
	return false;
}

void UWalletComponent::OnRep_Cash()
{
	OnCashUpdated.Broadcast();
}