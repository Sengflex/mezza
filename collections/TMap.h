#ifndef MZ_COLLTMAP_TMAP_H
#define MZ_COLLTMAP_TMAP_H
#include "../base/TMemMgr.h"
#include "../base/Exceptions.h"
#include "../base/TList.h"
#include "../str/TString.h"

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
  TBool valueIsObj;
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
TMap      *TMap_Create(TMemMgr *memmgr);

#define    TMap_SetEntry(MAP, KEY, VALUE) \
                TMap_SetEntry__Backend(MAP, KEY, VALUE, FALSE)
#define    TMap_SetEntryObj(MAP, KEY, VALUE) \
                TMap_SetEntry__Backend(MAP, KEY, VALUE, TRUE)

#define    TMap_UnsetEntry(MAP, KEY) TMap_UnsetEntry__Backend(MAP, KEY, NULL)
#define    TMap_UnsetEntryWithUserData(MAP, KEY, USERDATA) TMap_UnsetEntry__Backend(MAP, KEY, USERDATA)

void       TMap_UnsetEntry__Backend(TMap *map, char *key, void *userdata);

void      *TMap_GetEntry(TMap *map, char *key);

TLstNod   *TMap_GetEntryNode(TMap *map, char *key);

#define    TMapEntry_Create(MEMMGR, KEY, VAL) \
                TMapEntry_Create__Backend(MEMMGR, KEY, VAL, FALSE)
#define    TMapEntry_CreateObj(MEMMGR, KEY, VAL) \
                TMapEntry_Create__Backend(MEMMGR, KEY, VAL, TRUE)

TMapEntry *TMapEntry_Create__Backend(TMemMgr *memmgr, TString key,
                                        void *value, TBool valueIsObj);
void      *TMap_SetEntry__Backend(TMap *map, TString key, 
                                        void *value, TBool valueIsObj);

#endif /* MZ_COLLTMAP_TMAP_H */
