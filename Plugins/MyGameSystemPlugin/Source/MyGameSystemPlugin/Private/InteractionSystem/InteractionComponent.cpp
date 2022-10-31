// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/InteractionComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "InteractionSystem/InteractableActorInterface.h"
#include "InteractionSystem/InteractingActorInterface.h"
#include "Engine/CollisionProfile.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// This component is replicated by default.
	SetIsReplicatedByDefault(true);
	
}

// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initiate a tracing logic.
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
	// If this is a server...
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		// and an actor for interaction is valid...
		if (IsValid(GetActorForInteraction()))
		{
			// than interact with it.
			IInteractableActorInterface::Execute_Interact(GetActorForInteraction(), GetOwner());
		}
	}
}

void UInteractionComponent::SetIsTracing(bool NewIsTracing)
{
	// If this is a server...
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		// Set IsTracing.
		bIsTracing = NewIsTracing;

		// Start or end the tracing process.
		if (bIsTracing)
		{
			StartTracing();
		}
		else
		{
			EndTracing();
		}

		// Reset an actor for interaction.
		SetActorForInteraction(nullptr);
	}
}

bool UInteractionComponent::CanInteract() const
{
	return IsValid(GetActorForInteraction());
}

void UInteractionComponent::StartTracing()
{
	// If this is a server...
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		// than setup a timer for the tracing.
		GetWorld()->GetTimerManager().SetTimer(TraceTimer, this, &UInteractionComponent::Trace, TraceInterval, true);
	}
}

void UInteractionComponent::EndTracing()
{
	// Clear the trace timer.
	GetWorld()->GetTimerManager().ClearTimer(TraceTimer);
}

void UInteractionComponent::Trace()
{
	// If this is a server...
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		// and this component is attached to an interacting actor...
		if (GetOwner()->Implements<UInteractingActorInterface>())
		{
			// and a scene component for tracing of the owner is vaild...
			USceneComponent* TraceComponent = IInteractingActorInterface::Execute_GetTraceSceneComponent(GetOwner());
			if (IsValid(TraceComponent))
			{
				// Find points of a tracing line.
				FVector TraceStart = TraceComponent->GetComponentLocation() + TraceComponent->GetForwardVector() * TraceRadius;
				FVector TraceEnd = TraceStart + TraceComponent->GetForwardVector() * TraceLength;

				// Find actors to ignore.
				TArray<AActor*> ActorsToIgnore;
				GetOwner()->GetAllChildActors(ActorsToIgnore, true);
				ActorsToIgnore.Add(GetOwner());

				// Create a sphere collision.
				FCollisionShape CollisionShape = FCollisionShape::MakeSphere(TraceRadius);

				// Sweep the sphere.
				FHitResult OutHit;
				bool bWasHit = GetWorld()->SweepSingleByChannel(OutHit, TraceStart, TraceEnd, FQuat::Identity, TraceCollisionChannel, CollisionShape);

				// Draw debug lines if required.
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

				// If there is a hit...
				if (bWasHit && IsValid(OutHit.GetActor()))
				{
					// than set a hitted actor as a possible interactable.
					SetActorForInteraction(OutHit.GetActor());
				}
				else
				{
					// else there is nothing.
					SetActorForInteraction(nullptr);
				}
			}
		}
		else
		{
			// else this component can't trace.
			SetIsTracing(false);
		}
	}
}

void UInteractionComponent::SetActorForInteraction(class AActor* Actor)
{
	// If this is a server...
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		// and an actor for interaction isn't the same...
		if (Actor != ActorForInteraction)
		{
			// than set it.
			ActorForInteraction = Actor;

			// And notify the owning client about the available/unavailable interaction.
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

AActor* UInteractionComponent::GetActorForInteraction() const
{
	// If the actor for interaction is valid and implements the interactable interface...
	if (IsValid(ActorForInteraction) && ActorForInteraction->Implements<UInteractableActorInterface>())
	{
		// and an interaction is possible for the owner of this component...
		if (IInteractableActorInterface::Execute_CanInteract(ActorForInteraction, GetOwner()))
		{
			// than return the actor.
			return ActorForInteraction;
		}
	}

	// else don't.
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