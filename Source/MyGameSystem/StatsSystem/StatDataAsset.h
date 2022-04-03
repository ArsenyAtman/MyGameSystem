#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/ReplicableDataAsset/ReplicableDataAsset.h"
#include "StatDataAsset.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UStatDataAsset : public UReplicableDataAsset
{
	GENERATED_BODY()

public:

	virtual bool IsNameStableForNetworking() const override { return true; }

};