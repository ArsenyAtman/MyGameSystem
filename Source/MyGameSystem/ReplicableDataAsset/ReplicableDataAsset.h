#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ReplicableDataAsset.generated.h"

/** DataAsset class, that fixes DataAssets replication issue. (Warning: FNetGUIDCache::SupportsObject: SomeDataAsset NOT Supported.)*/
UCLASS()
class MYGAMESYSTEM_API UReplicableDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	virtual bool IsNameStableForNetworking() const override { return true; }

};