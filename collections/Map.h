
/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * collections/Map.h
 *
 *  Created on: 21 de dez de 2018
 *      Author: Danilo Almeida
 */

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
    define_exception(ExceptionTMapCreation, "Falha em criar o objeto TList")
    define_exception(ExceptionTMapSetEntry, "Falha em atribuir valor a entrada no mapa")
    define_exception(ExceptionTMapNotFoundEntry, "Entrada nao encontrada no mapa")
    define_exception(ExceptionTMapEntryCreation, "Falha em criar o objeto TMapEntry")
#endif

extern void *TREAT_VALUE_AS_OBJECT;

typedef int (*FMapFinder)(char *key, char *value, void *data);

typedef struct MapEntry {
  TString key;
  void *value;
  TBool valueIsObj;
} TMapEntry;

int        map_tlistcallback(TNode *node, void *tkntext);

#define    Map_SetEntry(MAP, KEY, VALUE) \
                Map_SetEntry__Backend(MAP, KEY, VALUE, FALSE, NULL)
#define    Map_SetEntryObj(MAP, KEY, VALUE) \
                Map_SetEntry__Backend(MAP, KEY, VALUE, TRUE, NULL)
#define    Map_SetEntryWithUserData(MAP, KEY, VALUE, USERDATA) \
                Map_SetEntry__Backend(MAP, KEY, VALUE, FALSE, USERDATA)
#define    Map_SetEntryObjWithUserData(MAP, KEY, VALUE, USERDATA) \
                Map_SetEntry__Backend(MAP, KEY, VALUE, TRUE, USERDATA)

#define    Map_UnsetEntry(MAP, KEY) Map_UnsetEntry__Backend(MAP, KEY, NULL)
#define    Map_UnsetEntryWithUserData(MAP, KEY, USERDATA) Map_UnsetEntry__Backend(MAP, KEY, USERDATA)

void       Map_UnsetEntry__Backend(TList *map, char *key, void *userdata);

void      *Map_GetEntry(TList *map, char *key);

#define    Map_GetEntryNode(MAP, KEY) TList_Find(MAP, map_tlistcallback, KEY)

void      *Map_SetEntry__Backend(TList *map, TString key, void *value, TBool valueIsObj, void *userdata);

#endif /* MZ_COLLTMAP_TMAP_H */
