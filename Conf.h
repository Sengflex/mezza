#ifndef MZ_CONF_H
#define MZ_CONF_H

#include "Type.h"
#include "base/TMemMgr.h"
#include "str/TString.h"
#include "base/Exceptions.h"
#include "base/TList.h"
#include "collections/TMap/TMap.h"

#ifndef MZ_TCONFSET_IMPLEMENT
    declare_exception(ExceptionConfLoad)
    declare_exception(ExceptionConfSave)
#else
    define_exception(ExceptionConfLoad, "Falha em carregar as configurações")
    define_exception(ExceptionConfSave, "Falha em salvar as configurações")
#endif

/**
 *
 *
 * Throws: ExceptionConfLoad
 */
TStatus Conf_LoadFromFile(TMap *confsMap, char *filename);
/**
 *
 *
 * Throws: ExceptionConfSave
 */
TStatus Conf_SaveToFile(TMap *confsMap, char *filename);

#endif /* MZ_CONF_H */
