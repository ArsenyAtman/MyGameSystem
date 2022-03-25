// Fill out your copyright notice in the Description page of Project Settings.

#include "TradeComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "MyGameSystem/InventorySystem/InventoryComponent.h"
#include "MyGameSystem/InventorySystem/Item.h"
#include "MyGameSystem/InventorySystem/ItemContainerInterface.h"
#include "WalletComponent.h"
#include "TradeActorInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UTradeComponent::UTradeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTradeComponent::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UTradeComponent, PartnerTrader, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(UTradeComponent, bIsTraiding, COND_OwnerOnly);
}

bool UTradeComponent::BeginTrade(UTradeComponent* WithTrader)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(WithTrader))
		{
			if (this->TradeBegins(WithTrader))
			{
				if (PartnerTrader->TradeBegins(this))
				{
					return true;
				}
				else
				{
					this->TradeEnds();
					return false;
				}
			}
		}
	}
	return false;
}

bool UTradeComponent::TradeBegins(UTradeComponent* WithTrader)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (!bIsTraiding)
		{
			PartnerTrader = WithTrader;
			bIsTraiding = true;
			OnTradeStarted.Broadcast();
			return true;
		}
	}
	return false;
}

bool UTradeComponent::Sell_Validate(UItem* Item, int Count)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Item) && Count >= 0)
		{
			return true;
		}
	}
	return false;
}

void UTradeComponent::Sell_Implementation(UItem* Item, int Count)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		Trade(Item, Count, ETradeAction::Sell);
	}
}

bool UTradeComponent::Purchase_Validate(UItem* Item, int Count)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Item) && Count >= 0)
		{
			return true;
		}
	}
	return false;
}

void UTradeComponent::Purchase_Implementation(UItem* Item, int Count)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		Trade(Item, Count, ETradeAction::Purchase);
	}
}

void UTradeComponent::EndTrade_Implementation()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		PartnerTrader->TradeEnds();
		TradeEnds();
	}
}

void UTradeComponent::TradeEnds()
{
	PartnerTrader = nullptr;
	bIsTraiding = false;
	OnTradeEnded.Broadcast();
}

TArray<FTradeOfferStruct> UTradeComponent::GetSellOffers(bool bCheckAvailability)
{
	TArray<FTradeOfferStruct> TradeOffers;

	if (bIsTraiding && IsValid(PartnerTrader))
	{
		if (GetOwner()->Implements<UTradeActorInterface>())
		{
			UInventoryComponent* Inventory = ITradeActorInterface::Execute_GetTradeInventoryComponent(GetOwner());
			if (IsValid(Inventory) && IsValid(PartnerTrader->GetPurchasePriceList()))
			{
				for (FTradeOfferStruct& Offer : GetTradeOffers(PartnerTrader->GetPurchasePriceList()))
				{
					if (Inventory->GetItemsCountOfClass(Offer.Item) > 0 || !bCheckAvailability)
					{
						TradeOffers.Add(Offer);
					}
				}
			}
		}
	}

	return TradeOffers;
}

TArray<FTradeOfferStruct> UTradeComponent::GetPurchaseOffers(bool bCheckAvailability)
{
	TArray<FTradeOfferStruct> TradeOffers;

	if (bIsTraiding && IsValid(PartnerTrader))
	{
		if (GetOwner()->Implements<UTradeActorInterface>())
		{
			UInventoryComponent* Inventory = ITradeActorInterface::Execute_GetTradeInventoryComponent(PartnerTrader->GetOwner());
			if (IsValid(Inventory) && IsValid(PartnerTrader->GetSellPriceList()))
			{
				for (FTradeOfferStruct& Offer : GetTradeOffers(PartnerTrader->GetSellPriceList()))
				{
					if (Inventory->GetItemsCountOfClass(Offer.Item) > 0 || !bCheckAvailability)
					{
						TradeOffers.Add(Offer);
					}
				}
			}
		}
	}

	return TradeOffers;
}

float UTradeComponent::GetMoney()
{
	AActor* Trader = this->GetOwner();
	if (Trader->Implements<UTradeActorInterface>())
	{
		UWalletComponent* Wallet = ITradeActorInterface::Execute_GetWalletComponent(Trader);
		if (IsValid(Wallet))
		{
			return Wallet->GetCash();
		}
	}
	return 0.0f;
}

float UTradeComponent::GetMoneyOfPartner()
{
	if (IsValid(PartnerTrader))
	{
		AActor* Trader = PartnerTrader->GetOwner();
		if (Trader->Implements<UTradeActorInterface>())
		{
			UWalletComponent* Wallet = ITradeActorInterface::Execute_GetWalletComponent(Trader);
			if (IsValid(Wallet))
			{
				return Wallet->GetCash();
			}
		}
	}
	return 0.0f;
}

void UTradeComponent::Trade(UItem* Item, int Count, ETradeAction TradeAction)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (bIsTraiding && IsValid(PartnerTrader))
		{
			AActor* Seller = nullptr;
			AActor* Purchaser = nullptr;
			UDataTable* PriceList = nullptr;

			if (TradeAction == ETradeAction::Sell)
			{
				Seller = this->GetOwner();
				Purchaser = PartnerTrader->GetOwner();
				PriceList = PartnerTrader->GetPurchasePriceList();
			}
			else if (TradeAction == ETradeAction::Purchase)
			{
				Seller = PartnerTrader->GetOwner();
				Purchaser = this->GetOwner();
				PriceList = PartnerTrader->GetSellPriceList();
			}

			if (Seller->Implements<UTradeActorInterface>() && Purchaser->Implements<UTradeActorInterface>())
			{
				UInventoryComponent* SellerInventory = ITradeActorInterface::Execute_GetTradeInventoryComponent(Seller);
				UWalletComponent* SellerWallet = ITradeActorInterface::Execute_GetWalletComponent(Seller);
				UInventoryComponent* PurchaserInventory = ITradeActorInterface::Execute_GetTradeInventoryComponent(Purchaser);
				UWalletComponent* PurchaserWallet = ITradeActorInterface::Execute_GetWalletComponent(Purchaser);

				if (IsValid(PriceList) && IsValid(Item) && Count >= 0 && IsValid(SellerInventory) && IsValid(SellerWallet) && IsValid(PurchaserInventory) && IsValid(PurchaserWallet))
				{
					for (FTradeOfferStruct& Offer : GetTradeOffers(PriceList))
					{
						if (Offer.Item == Item)
						{
							float FinalPrice = Offer.Price * Count;
							if (PurchaserWallet->GetCash() >= FinalPrice)
							{
								if (GetCountOfItemsInInventory(SellerInventory, Item) >= Count)
								{
									//UKismetSystemLibrary::PrintString(GetWorld(), FString::FromInt(PurchaserInventory->GetInventorySize() - PurchaserInventory->GetItemsCount()), true, true, FLinearColor::Blue);
									if (PurchaserInventory->GetInventorySize() - PurchaserInventory->GetItemsCount() >= Count)
									{
										ExtractItemsFromInventory(SellerInventory, Item, Count);
										AddItemsToInventory(PurchaserInventory, Item, Count);
										PurchaserWallet->Transaction(SellerWallet, FinalPrice);

										if (TradeAction == ETradeAction::Sell)
										{
											ItemsSold();
										}
										else if (TradeAction == ETradeAction::Purchase)
										{
											ItemsPurchased();
										}
									}
									else
									{
										if (TradeAction == ETradeAction::Sell)
										{
											NotEnoughSpaceInInventoryOfPartner();
										}
										else if (TradeAction == ETradeAction::Purchase)
										{
											NotEnoughSpaceInInventory();
										}
									}
								}
								else
								{
									if (TradeAction == ETradeAction::Sell)
									{
										NotEnoughItems();
									}
									else if (TradeAction == ETradeAction::Purchase)
									{
										NotEnoughItemsOfPartner();
									}
								}
							}
							else
							{
								if (TradeAction == ETradeAction::Sell)
								{
									NotEnoughMoneyOfPartner();
								}
								else if (TradeAction == ETradeAction::Purchase)
								{
									NotEnoughMoney();
								}
							}
						}
					}
				}
			}
		}
	}
}

int UTradeComponent::GetCountOfItemsInInventory(UInventoryComponent* Inventory, UItem* Item) const
{
	int Count = 0;
	if (IsValid(Inventory) && IsValid(Item))
	{
		for (FItemStruct& ItemStruct : IItemContainerInterface::Execute_GetItems(Inventory))
		{
			if (ItemStruct.Item == Item)
			{
				Count++;
			}
		}
	}
	return Count;
}

int UTradeComponent::ExtractItemsFromInventory(UInventoryComponent* Inventory, UItem* Item, int Count)
{
	int ExtractedCount = 0;
	if (IsValid(Inventory) && IsValid(Item))
	{
		for (; ExtractedCount < Count; ExtractedCount++)
		{
			if (!Inventory->RemoveItemOfClass(Item))
			{
				break;
			}
		}
	}
	return ExtractedCount;
}

int UTradeComponent::AddItemsToInventory(UInventoryComponent* Inventory, UItem* Item, int Count)
{
	int AddedCount = 0;
	if (IsValid(Inventory) && IsValid(Item))
	{
		for (; AddedCount < Count; AddedCount++)
		{
			if (!Inventory->IItemContainerInterface::Execute_AddItem(Inventory, FItemStruct(Item), -1))
			{
				break;
			}
		}
	}
	return AddedCount;
}

TArray<FTradeOfferStruct> UTradeComponent::GetTradeOffers(UDataTable* PriceList) const
{
	TArray<FTradeOfferStruct*> PriceListOffers;
	PriceList->GetAllRows<FTradeOfferStruct>(FString("None"), PriceListOffers);
	TArray<FTradeOfferStruct> Offers;
	for (FTradeOfferStruct* Offer : PriceListOffers)
	{
		Offers.Add(*Offer);
	}
	return Offers;
}

void UTradeComponent::OnRep_IsTraiding()
{
	if (bIsTraiding)
	{
		OnTradeStarted.Broadcast();
	}
	else
	{
		OnTradeEnded.Broadcast();
	}
}

void UTradeComponent::ItemsSold_Implementation()
{
	OnItemsSold.Broadcast();
}

void UTradeComponent::ItemsPurchased_Implementation()
{
	OnItemsPurchased.Broadcast();
}

void UTradeComponent::NotEnoughMoney_Implementation()
{
	OnNotEnoughMoney.Broadcast();
}

void UTradeComponent::NotEnoughMoneyOfPartner_Implementation()
{
	OnNotEnoughMoneyOfPartner.Broadcast();
}

void UTradeComponent::NotEnoughItems_Implementation()
{
	OnNotEnoughItems.Broadcast();
}

void UTradeComponent::NotEnoughItemsOfPartner_Implementation()
{
	OnNotEnoughItemsOfPartner.Broadcast();
}

void UTradeComponent::NotEnoughSpaceInInventory_Implementation()
{
	OnNotEnoughSpaceInInventory.Broadcast();
}

void UTradeComponent::NotEnoughSpaceInInventoryOfPartner_Implementation()
{
	OnNotEnoughSpaceInInventoryOfPartner.Broadcast();
}