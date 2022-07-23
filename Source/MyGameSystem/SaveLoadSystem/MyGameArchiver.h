#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "WCSaveGameArchive.generated.h"


struct FWCSaveGameArchive : public FObjectAndNameAsStringProxyArchive
{
    FWCSaveGameArchive(FArchive& InInnerArchive) 
             : FObjectAndNameAsStringProxyArchive(InInnerArchive,true)
    {
        ArIsSaveGame = true;
                ArNoDelta = true; // Optional, useful when saving/loading variables without resetting the level.
                                  // Serialize variables even if weren't modified and mantain their default values.
    }
};