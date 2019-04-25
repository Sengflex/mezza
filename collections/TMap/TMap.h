#ifndef MZ_COLLTMAP_TMAP_H
#define MZ_COLLTMAP_TMAP_H
#include "../../base/TMemMgr.h"
#include "../../base/Exceptions.h"
#include "../../base/TList.h"
#include "../../str/TString.h"

#ifndef TMAP_IMPLEMENT
    declare_exception(ExceptionTMapCreation)
    declare_exception(ExceptionTMapSetEntry)
    declare_exception(ExceptionTMapNotFoundEntry)
    declare_exception(ExceptionTMapEntryCreation)
#else
    define_exception(ExceptionTMapCreation, "Falha em criar o objeto TMap")
    define_exception(ExceptionTMapSetEntry, "Falha em atribuir valor a entrada no mapa")
    define_exception(ExceptionTMapNotFoundEntry, "Entrada nao encontrada no mapa")
    define_exception(ExceptionTMapEntryCreation, "Falha em criar o objeto TMapEntry")
#endif

extern void *TREAT_VALUE_AS_OBJECT;

typedef int (*FMapFinder)(char *key, char *value, void *data);

typedef struct Map {
    TList *entries;
    TSize  entriesSize;
} TMap;

typedef struct MapEntry {
  TString key;
  void *value;
} TMapEntry;

/**
 * Cria e retorna um mapa
 * 
 * Parâmetros:
 * - memmgr: Gerenciador de memória
 * 
 * Retorno:
 * NULL em caso de erro.
 * 
 * Exceções:
 * ExceptionTMapCreation
 * 
 * Destruição: 
 * Ao chamar TObject_Destroy, o segundo parâmetro deve 
 * ser TREAT_VALUE_AS_OBJECT caso se deseje que os valores nos pares 
 * sejam tratados como objetos e destruído assim como as chaves são 
 * por padrão
*/
TMap    *TMap_Create(TMemMgr *memmgr);

void    *TMap_SetEntry(TMap *map, TString key, void *value);

void     TMap_UnsetEntry(TMap *map, TString key);

void    *TMap_GetEntry(TMap *map, TString key);

TLstNod *TMap_GetEntryNode(TMap *map, TString key);

TMapEntry *TMapEntry_Create(TMemMgr *memmgr, TString key,
                                            void *value);

#endif /* MZ_COLLTMAP_TMAP_H */
