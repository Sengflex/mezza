#ifndef MZ_TSTRINGVECTOR_H
#define MZ_TSTRINGVECTOR_H

#include "../base/TMemMgr.h"
#include "../base/Exceptions.h"

#include "../str/TString.h"

typedef TString *TStringVector;

#ifndef MZ_TSTRINGVECTOR_H_IMPLEMENT
    declare_exception(ExceptionTStringVectorCreation)
#else
    define_exception(ExceptionTStringVectorCreation, "Falha em criar o vetor de strings")
#endif

/**
 *
 * Throws: ExceptionTStringVectorCreation
 * */
TStringVector   TStringVector_Create(TMemMgr *memmgr, TString *orig,
                                        TCount nofPos);

void            TStringVector_Reset(TString *vstr, TSize vstrSz);


#endif /* MZ_TSTRINGVECTOR_H */
