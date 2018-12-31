#include "TMap.h"
#include "../../str/TString.h"

void    *TMap_SetEntry_Strvd(TMap *map, TString key, void *value);
void     TMap_UnsetEntry_Strvd(TMap *map, TString key);
void    *TMap_GetEntry_Strvd(TMap *map, TString key);
TLstNod *TMap_GetEntryNode_Strvd(TMap *map, TString key);
