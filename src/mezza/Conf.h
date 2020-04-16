
/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * Conf.h
 *
 *  Created on: before v.2.0
 *      Author: danilo
 */

#ifndef MZ_CONF_H
#define MZ_CONF_H

#include "Type.h"
#include "base/TMemMgr.h"
#include "str/TString.h"
#include "base/Exceptions.h"
#include "base/TList.h"
#include "collections/Map.h"

#ifndef MZ_CONF_IMPLEMENT
    declare_exception(ExceptionConfLoad)
    declare_exception(ExceptionConfSave)
    declare_exception(ExceptionConfSet)
    declare_exception(ExceptionConfNotFound)
#else
    define_exception(ExceptionConfLoad, "Falha em carregar as configurações")
    define_exception(ExceptionConfSave, "Falha em salvar as configurações")
    define_exception(ExceptionConfSet, "Falha em adicionar configuração")
    define_exception(ExceptionConfNotFound, "Configuração não encontrada")
#endif

/**
 *
 *
 * Dispara: ExceptionConfLoad
 */
TStatus Conf_LoadFromFile(TList *confsMap, char *filename);
/**
 *
 *
 * Dispara: ExceptionConfSave
 */
TStatus Conf_SaveToFile(TList *confsMap, char *filename);
/**
 * 
 * Exceções:
 * ExceptionConfSet
*/
TStatus Conf_Set(TList *confsMap, char *conf, char *value);

TString Conf_Get(TList *confsMap, char *conf);

#define Conf_Unset(CNFMAP, KEY) Map_UnsetEntry(CNFMAP, KEY)

#endif /* MZ_CONF_H */
