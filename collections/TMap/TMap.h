#ifndef MZ_COLLTMAP_TMAP_H
#define MZ_COLLTMAP_TMAP_H
#include "../../base/TMemMgr.h"
#include "../../base/Exceptions.h"
#include "../../base/TList.h"
#include "../../str/TString.h"

#ifndef MZ_TMAP_IMPLEMENT
    declare_exception(ExceptionTMapCreation)
    declare_exception(ExceptionTMapSetEntry)
    declare_exception(ExceptionTMapNotFoundEntry)
#else
    define_exception(ExceptionTMapCreation, "Falha em criar o objeto TMap")
    define_exception(ExceptionTMapSetEntry, "Falha em atribuir valor a entrada no mapa")
    define_exception(ExceptionTMapNotFoundEntry, "Entrada nao encontrada no mapa")
#endif

typedef int (*FMapFinder)(char *key, char *value, void *data);

typedef struct Map {
    TList *entries;
    TSize  entriesSize;
} TMap;

TMap *TMap_Create(TMemMgr *memmgr);

#endif /* MZ_COLLTMAP_TMAP_H */
