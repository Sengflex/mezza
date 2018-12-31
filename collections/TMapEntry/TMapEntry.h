#ifndef MZ_COLLTMAPENTRY_TMAPENTRY
#define  MZ_COLLTMAPENTRY_TMAPENTRY

#include "../../base/Exceptions.h"

#ifndef MZ_TMAPENTRY_STRVD_IMPLEMENT
    declare_exception(ExceptionTMapEntryCreation)
#else
    define_exception(ExceptionTMapEntryCreation, "Falha em criar o objeto TMapEntry")
#endif

#endif /*  MZ_COLLTMAPENTRY_TMAPENTRY */
