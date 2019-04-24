#ifndef MZ_STRINGBUILDER_H
#define MZ_STRINGBUILDER_H

#include <stdio.h>

#include "../base/TMemMgr.h"

#include "../str/TString.h"
#include "../base/Exceptions.h"

#ifndef MZ_STRINGBUILDER_IMPLEMENT
    declare_exception(ExceptionStringBuilder)
#else
    define_exception(ExceptionStringBuilder, "Falha no construtor de string")
#endif

/**
 *
 * Dispara: ExceptionStringBuilder
 * */
TString StringBuilder_FromString ( TMemMgr *memmgr, char* src, char *format, ... );
/**
 *
 * Dispara: ExceptionStringBuilder
 * */
TString StringBuilder_FromFile ( TMemMgr *memmgr, TString *str, FILE *fp );
/**
 *
 * Dispara: ExceptionStringBuilder
 * */
TString StringBuilder_FromFileSection ( TMemMgr *memmgr, TString *str, FILE *fp, int sep );

#endif /* MZ_STRINGBUILDER_H */
