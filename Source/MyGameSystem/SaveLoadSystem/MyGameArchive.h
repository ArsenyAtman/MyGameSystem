
#pragma once

#include "CoreMinimal.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

//#include "MyGameArchive.generated.h"


struct MYGAMESYSTEM_API FMyGameArchive : public FObjectAndNameAsStringProxyArchive
{
    FMyGameArchive(FArchive& InInnerArchive) : FObjectAndNameAsStringProxyArchive(InInnerArchive,true)
    {
        ArIsSaveGame = true;
        ArNoDelta = true;   // Optional, useful when saving/loading variables without resetting the level.
                            // Serialize variables even if weren't modified and mantain their default values.
    }
};
