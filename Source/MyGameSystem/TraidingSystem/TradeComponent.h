// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "TradeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTradeComponentConditionDelegate);

UENUM(BlueprintType, Blueprintable)
enum class ETradeAction : uint8
{
	Sell		UMETA(DisplayName = "Sell"),
	Purchase	UMETA(DisplayName = "Purchase"),
};

USTRUCT(BlueprintType, Blueprintable)
struct FTradeOfferStruct : public FTableRowBase
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UItem* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Price;

	FTradeOfferStruct(class UItem* OfferedItem = nullptr, float OfferedPrice = 0.0f)
	{
		Item = OfferedItem;
		Price = OfferedPrice;
	}

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UTradeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTradeComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	bool BeginTrade(class UTradeComponent* WithTrader);

	UFUNCTION(BlueprintCallable)
	bool TradeBegins(class UTradeComponent* WithTrader);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Sell(class UItem* Item, int Count);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Purchase(class UItem* Item, int Count);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void EndTrade();

	UFUNCTION(BlueprintCallable)
	void TradeEnds();

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UDataTable* GetSellPriceList() { return SellPriceList; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UDataTable* GetPurchasePriceList() { return PurchasePriceList; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsTraiding() { return bIsTraiding; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UTradeComponent* GetPartnerTrader() { return PartnerTrader; }

	UFUNCTION(BlueprintPure)
	TArray<FTradeOfferStruct> GetSellOffers(bool bCheckAvailability = true); // was FORCEINLINE

	UFUNCTION(BlueprintPure)
	TArray<FTradeOfferStruct> GetPurchaseOffers(bool bCheckAvailability = true); // was FORCEINLINE

	UFUNCTION(BlueprintPure)
	float GetMoney(); // was FORCEINLINE

	UFUNCTION(BlueprintPure)
	float GetMoneyOfPartner(); // was FORCEINLINE

	UPROPERTY(BlueprintAssignable)
	FTradeComponentConditionDelegate OnTradeStarted;

	UPROPERTY(BlueprintAssignable)
	FTradeComponentConditionDelegate OnTradeEnded;

	UPROPERTY(BlueprintAssignable)
	FTradeComponentConditionDelegate OnItemsSold;

	UPROPERTY(BlueprintAssignable)
	FTradeComponentConditionDelegate OnItemsPurchased;

	UPROPERTY(BlueprintAssignable)
	FTradeComponentConditionDelegate OnNotEnoughMoney;

	UPROPERTY(BlueprintAssignable)
	FTradeComponentConditionDelegate OnNotEnoughMoneyOfPartner;

	UPROPERTY(BlueprintAssignable)
	FTradeComponentConditionDelegate OnNotEnoughItems;

	UPROPERTY(BlueprintAssignable)
	FTradeComponentConditionDelegate OnNotEnoughItemsOfPartner;

	UPROPERTY(BlueprintAssignable)
	FTradeComponentConditionDelegate OnNotEnoughSpaceInInventory;

	UPROPERTY(BlueprintAssignable)
	FTradeComponentConditionDelegate OnNotEnoughSpaceInInventoryOfPartner;

protected:

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* SellPriceList;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* PurchasePriceList;

	UFUNCTION(BlueprintCallable)
	void Trade(class UItem* Item, int Count, enum ETradeAction TradeAction);

	UFUNCTION(BlueprintPure)
	int GetCountOfItemsInInventory(class UInventoryComponent* Inventory, class UItem* Item) const;

	UFUNCTION(BlueprintCallable)
	int ExtractItemsFromInventory(class UInventoryComponent* Inventory, class UItem* Item, int Count);

	UFUNCTION(BlueprintCallable)
	int AddItemsToInventory(class UInventoryComponent* Inventory, class UItem* Item, int Count);

	UFUNCTION(BlueprintPure)
	TArray<FTradeOfferStruct> GetTradeOffers(class UDataTable* PriceList) const;

private:	
	
	UPROPERTY(ReplicatedUsing = OnRep_IsTraiding)
	bool bIsTraiding = false;

	UFUNCTION()
	void OnRep_IsTraiding();

	UPROPERTY(Replicated)
	class UTradeComponent* PartnerTrader;

	UFUNCTION(NetMulticast, Reliable)
	void ItemsSold();

	UFUNCTION(NetMulticast, Reliable)
	void ItemsPurchased();

	UFUNCTION(NetMulticast, Reliable)
	void NotEnoughMoney();

	UFUNCTION(NetMulticast, Reliable)
	void NotEnoughMoneyOfPartner();

	UFUNCTION(NetMulticast, Reliable)
	void NotEnoughItems();

	UFUNCTION(NetMulticast, Reliable)
	void NotEnoughItemsOfPartner();

	UFUNCTION(NetMulticast, Reliable)
	void NotEnoughSpaceInInventory();

	UFUNCTION(NetMulticast, Reliable)
	void NotEnoughSpaceInInventoryOfPartner();
		
};
