// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TradeActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTradeActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYGAMESYSTEM_API ITradeActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TradeActorInterface")
	class UTradeComponent* GetTradeComponent();
	virtual class UTradeComponent* GetTradeComponent_Implementation() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TradeActorInterface")
	class UWalletComponent* GetWalletComponent();
	virtual class UWalletComponent* GetWalletComponent_Implementation() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TradeActorInterface")
	class UInventoryComponent* GetTradeInventoryComponent();
	virtual class UInventoryComponent* GetTradeInventoryComponent_Implementation() = 0;
};
