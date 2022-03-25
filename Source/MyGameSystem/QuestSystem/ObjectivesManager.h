// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectivesManager.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FObjectivesOnLevel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UObjective> ObjectiveClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class AActor*> RelatedOnLevelObjectives;

	FObjectivesOnLevel()
	{
	}
};

UCLASS()
class MYGAMESYSTEM_API AObjectivesManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectivesManager();

	UFUNCTION(BlueprintCallable)
	TArray<class AActor*> GetOnLevelObjectives(class UObjective* ForObjective);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<struct FObjectivesOnLevel> Objectives;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
