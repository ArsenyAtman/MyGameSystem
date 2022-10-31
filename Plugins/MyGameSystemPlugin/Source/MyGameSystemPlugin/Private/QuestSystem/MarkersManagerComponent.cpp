// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem/MarkersManagerComponent.h"

#include "QuestSystem/MarkableActorInterface.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
UMarkersManagerComponent::UMarkersManagerComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

}

void UMarkersManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DeleteAllMarkersLocally();

	Super::EndPlay(EndPlayReason);
}

void UMarkersManagerComponent::MarkActors_Implementation(const TArray<AActor*>& ActorsToMark)
{
	MarkActorsLocally(ActorsToMark);
}

void UMarkersManagerComponent::DeleteAllMarkers_Implementation()
{
	DeleteAllMarkersLocally();
}

void UMarkersManagerComponent::MarkActorsLocally(const TArray<AActor*>& ActorsToMark)
{
	DeleteAllMarkersLocally();

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
}

void UMarkersManagerComponent::DeleteAllMarkersLocally()
{
	for (int16 i = Markers.Num() - 1; i >= 0; --i)
	{
		auto Marker = Markers[i];
		if (IsValid(Marker))
		{
			Marker->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
			Marker->Destroy();
		}
	}

	Markers.Empty();
}
