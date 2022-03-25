// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "InteractableActorInterface.h"
#include "InteractingActorInterface.h"
#include "Engine/CollisionProfile.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		SetIsTracing(bIsTracing);
	}

}

void UInteractionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInteractionComponent, bIsTracing);
}

void UInteractionComponent::Interact_Implementation()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(GetActorForInteraction()))
		{
			IInteractableActorInterface::Execute_Interact(GetActorForInteraction(), GetOwner());
		}
	}
}

void UInteractionComponent::SetIsTracing(bool NewIsTracing)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		bIsTracing = NewIsTracing;

		if (bIsTracing)
		{
			StartTracing();
		}
		else
		{
			EndTracing();
		}

		SetActorForInteraction(nullptr);
	}
}

bool UInteractionComponent::GetIsTracing()
{
	return bIsTracing;
}

bool UInteractionComponent::CanInteract()
{
	return IsValid(GetActorForInteraction());
}

void UInteractionComponent::StartTracing()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		GetWorld()->GetTimerManager().SetTimer(TraceTimer, this, &UInteractionComponent::Trace, TraceInterval, true);
	}
}

void UInteractionComponent::EndTracing()
{
	GetWorld()->GetTimerManager().ClearTimer(TraceTimer);
}

void UInteractionComponent::Trace()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (GetOwner()->Implements<UInteractingActorInterface>())
		{
			USceneComponent* TraceComponent = IInteractingActorInterface::Execute_GetTraceSceneComponent(GetOwner());

			if (IsValid(TraceComponent))
			{
				FVector TraceStart = TraceComponent->GetComponentLocation() + TraceComponent->GetForwardVector() * TraceRadius;
				FVector TraceEnd = TraceStart + TraceComponent->GetForwardVector() * TraceLength;

				TArray<AActor*> ActorsToIgnore;
				GetOwner()->GetAllChildActors(ActorsToIgnore, true);
				ActorsToIgnore.Add(GetOwner());

				FHitResult OutHit;
				FCollisionShape CollisionShape = FCollisionShape::MakeSphere(TraceRadius);
				bool bWasHit = GetWorld()->SweepSingleByChannel(OutHit, TraceStart, TraceEnd, FQuat::Identity, TraceCollisionChannel, CollisionShape);

				if (bDrawDebugTrace)
				{
					if (bWasHit)
					{
						UKismetSystemLibrary::DrawDebugLine(GetWorld(), OutHit.TraceStart, OutHit.ImpactPoint, FLinearColor::Red);
						UKismetSystemLibrary::DrawDebugLine(GetWorld(), OutHit.ImpactPoint, OutHit.TraceEnd, FLinearColor::Green);
					}
					else
					{
						UKismetSystemLibrary::DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FLinearColor::Red);
					}
				}

				// = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TraceStart, TraceEnd, TraceRadius, UCollisionProfile::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorsToIgnore, ( ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None), OutHit, true);

				if (bWasHit && OutHit.Actor.IsValid())
				{
					SetActorForInteraction(OutHit.Actor.Get());
				}
				else
				{
					SetActorForInteraction(nullptr);
				}
			}
		}
		else
		{
			SetIsTracing(false);
		}
	}
}

void UInteractionComponent::SetActorForInteraction(class AActor* Actor)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (Actor != ActorForInteraction)
		{
			ActorForInteraction = Actor;

			if (IsValid(GetActorForInteraction()))
			{
				InteractionAvailable(IInteractableActorInterface::Execute_GetActionDescription(Actor));
			}
			else
			{
				InteractionUnavailable();
			}
		}
	}
}

AActor* UInteractionComponent::GetActorForInteraction()
{
	if (IsValid(ActorForInteraction) && ActorForInteraction->Implements<UInteractableActorInterface>())
	{
		if (IInteractableActorInterface::Execute_CanInteract(ActorForInteraction, GetOwner()))
		{
			return ActorForInteraction;
		}
	}

	return nullptr;
}

void UInteractionComponent::InteractionAvailable_Implementation(const FText& Description)
{
	if (OnInteractionAvailable.IsBound())
	{
		OnInteractionAvailable.Broadcast(Description);
	}
}

void UInteractionComponent::InteractionUnavailable_Implementation()
{
	if (OnInteractionUnavailable.IsBound())
	{
		OnInteractionUnavailable.Broadcast();
	}
}