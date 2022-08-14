// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorWithQuestsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActorWithQuestsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * An actor with QuestComponent.
 */
class MYGAMESYSTEM_API IActorWithQuestsInterface
{
	GENERATED_BODY()

public:

	/**
	 * Returns QuestComponent of the actor.
	 * @return QuestComponent of this actor.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorWithQuestsInterface")
	class UQuestComponent* GetActorQuestComponent() const;
	virtual class UQuestComponent* GetActorQuestComponent_Implementation() const { return nullptr; }

};
