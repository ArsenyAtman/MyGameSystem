// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveMarkersManager.h"
#include "MarkableActorInterface.h"

// Sets default values
AObjectiveMarkersManager::AObjectiveMarkersManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

TArray<AActor*> AObjectiveMarkersManager::MarkActors(TSubclassOf<AActor> MarkerClass, const TArray<AActor*>& ActorsToMark)
{
	TArray<AActor*> NewMarkers;

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
			NewMarkers.Add(NewMarker);
		}
	}

	return NewMarkers;
}

TArray<AActor*> AObjectiveMarkersManager::UnmarkActors(TSubclassOf<AActor> MarkerClass, const TArray<AActor*>& ActorsToUnmark)
{
	TArray<AActor*> RemovedMarkers;

	for (int i = Markers.Num() - 1; i >= 0; --i)
	{
		auto Marker = Markers[i];
		if (IsValid(Marker) && (!IsValid(Marker->GetAttachParentActor()) || (Marker->GetClass() == MarkerClass && ActorsToUnmark.Find(Marker->GetAttachParentActor()) != INDEX_NONE)))
		{
			Marker->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
			Marker->Destroy();

			RemovedMarkers.Add(Marker);
		}
		Markers.Remove(Marker);
	}

	return RemovedMarkers;
}
