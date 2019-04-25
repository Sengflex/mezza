#ifndef MZ_CONF_H
#define MZ_CONF_H

#include "Type.h"
#include "base/TMemMgr.h"
#include "str/TString.h"
#include "base/Exceptions.h"
#include "base/TList.h"
#include "collections/TMap.h"

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
TStatus Conf_LoadFromFile(TMap *confsMap, char *filename);
/**
 *
 *
 * Dispara: ExceptionConfSave
 */
TStatus Conf_SaveToFile(TMap *confsMap, char *filename);
/**
 * 
 * Exceções:
 * ExceptionConfSet
*/
TStatus Conf_Set(TMap *confsMap, char *conf, char *value);

TString Conf_Get(TMap *confsMap, char *conf);

#endif /* MZ_CONF_H */
