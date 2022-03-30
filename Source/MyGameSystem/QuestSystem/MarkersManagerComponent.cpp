// Fill out your copyright notice in the Description page of Project Settings.


#include "MarkersManagerComponent.h"
#include "MarkableActorInterface.h"

// Sets default values
UMarkersManagerComponent::UMarkersManagerComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;

}

TArray<AActor*> UMarkersManagerComponent::MarkActorsReplicated(TSubclassOf<AActor> MarkerClass, const TArray<AActor*>& ActorsToMark)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		MarkActorsOnClients(MarkerClass, ActorsToMark);
		return MarkActorsLocally(MarkerClass, ActorsToMark);
	}
	return TArray<AActor*>();
}

TArray<AActor*> UMarkersManagerComponent::MarkActorsLocally(TSubclassOf<AActor> MarkerClass, const TArray<AActor*>& ActorsToMark)
{
	TArray<AActor*> Markers;

	for (AActor* ActorToMark : ActorsToMark)
	{
		AActor* NewMarker = GetWorld()->SpawnActor<AActor>(MarkerClass, FTransform());
		if (IsValid(NewMarker))
		{
			NewMarker->AttachToActor(ActorToMark, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
			if (ActorToMark->Implements<UMarkableActorInterface>())
			{
				FTransform MarkerTransfrom = IMarkableActorInterface::Execute_GetMarkerRelativeTransform(ActorToMark);
				NewMarker->SetActorRelativeTransform(MarkerTransfrom);
			}
			Markers.Add(NewMarker);
		}
	}

	return Markers;
}

void UMarkersManagerComponent::MarkActorsOnClients_Implementation(TSubclassOf<class AActor> MarkerClass, const TArray<class AActor*>& ActorsToMark)
{
	if (GetOwnerRole() != ENetRole::ROLE_Authority)
	{
		MarkActorsLocally(MarkerClass, ActorsToMark);
	}
}

void UMarkersManagerComponent::UnmarkActors(const TArray<AActor*>& Markers)
{
	for (int i = Markers.Num() - 1; i >= 0; --i)
	{
		auto Marker = Markers[i];
		if (IsValid(Marker))
		{
			Marker->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
			Marker->Destroy();
		}
	}
}
