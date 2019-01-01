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

extern void *TREAT_VALUE_AS_OBJECT;

typedef int (*FMapFinder)(char *key, char *value, void *data);

typedef struct Map {
    TList *entries;
    TSize  entriesSize;
} TMap;
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
TMap *TMap_Create(TMemMgr *memmgr);

#endif /* MZ_COLLTMAP_TMAP_H */
