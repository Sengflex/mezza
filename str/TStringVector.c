#define MZ_TSTRINGVECTOR_H_IMPLEMENT
#include "TStringVector.h"

#include "../Lang.h"
#include "../OnError.h"

#include "../base/TObject.h"

static void TStringVector_dtor(TObject addr, void *ignore);

TStringVector   TStringVector_Create(TMemMgr *memmgr, TString *orig,
                                        TCount nofPos) {
    TStringVector ret = TObject_CreateZero(memmgr, sizeof(TString) * nofPos, TStringVector_dtor);

    onerror( ret )
		throw(ExceptionTStringVectorCreation, NULL)

    if(orig) {
        loop_i(nofPos)
            ret[_INDEX_] = TString_Create(memmgr, orig[_INDEX_], 0);
        	onerror( ret[_INDEX_] ) {
            	TObject_Destroy(ret, NULL);
        		throw(ExceptionTStringVectorCreation, NULL)
            }
        end_loop_i
    }

    return ret;
}

void     TStringVector_Reset(TStringVector strvec, TSize strvecSz) {
    loop_i(strvecSz)
        strvec[_INDEX_] = NULL;
    end_loop_i
}



static void TStringVector_dtor(TObject addr, void *ignore) {
    TString *vstr = addr;
    TSize vstrSz = TObject_SizeOf(addr) / sizeof(TString);

    loop_i(vstrSz)
        if(vstr[_INDEX_])
            TObject_Destroy(vstr[_INDEX_], NULL);
    end_loop_i

}
