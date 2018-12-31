#ifndef MZ_COLLTMAPENTRY_STRVD_H
#define MZ_COLLTMAPENTRY_STRVD_H


#include "../../base/TMemMgr.h"
#include "../../str/TString.h"

typedef struct MapEntryStrVoid {
  TString key;
  void *value;
} TMapEntryStrVoid;

TMapEntryStrVoid *TMapEntryStrVoid_Create(TMemMgr *memmgr, TString key,
                                            void *value);

#endif /* MZ_COLLTMAPENTRY_STRVD_H */
