
/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * base/TObject.h
 *
 *  Created on:  9 de nov de 2018
 *      Author: Danilo Almeida
 */

#define MZ_BASE_OBJECT_H_IMPLEMENT
#include "TObject.h"

#include "TMemMgr.h"
#include "../OnError.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ObjMta TObjMta;

struct ObjMta {
    TObject  addr;

    FObjDestructor
             cleaner;
    TMemMgr *memmgr;
};

TObject  TObject_Create(TMemMgr *memmgr, TSize sizeInBytes, FObjDestructor cleaner) {
    TObjMta *ometa = (TObjMta *)TMemMgr_Alloc(memmgr, sizeof(TObjMta) + sizeInBytes);

    onerror( ometa )
        throw_note(ExceptionTObjectCreation, NULL, "Allocação de memória falhou")

    ometa->addr = (TObject*)(ometa + 1);
    ometa->cleaner = cleaner;
    ometa->memmgr = memmgr;

    return ometa->addr;
}

TObject  TObject_CreateZero(TMemMgr *memmgr, TSize sizeInBytes, FObjDestructor cleaner) {
    TObject ret;

    try(create_zero_try001)
		ret = TObject_Create(memmgr, sizeInBytes, cleaner); check(create_zero_try001)

    	memset(ret, 0, sizeInBytes);
	catch(create_zero_try001)
		on(Exception)
			rethrow_it(NULL)
		end_on
	end_try(create_zero_try001)

    return ret;
}

TObject  TObject_SetDtor   (TObject self, FObjDestructor dtor) {
    TObjMta *omta = ((TObjMta *)self)-1;

    omta->cleaner = dtor;

    return self;
}

static TObject destroy_backend(TObject addr, void *extra, TBool bypass) {

    if(addr) {
        TObjMta *obj = ((TObjMta*)addr) - 1;

        if(!bypass) {
            if(obj->cleaner)
                obj->cleaner(addr, extra);
        }

        TMemMgr_Free(obj->memmgr, obj);
    }

    return NULL;
}

TObject TObject_Destroy(TObject addr, void *extra) {
    return destroy_backend(addr, extra, FALSE);
}

TObject TObject_DestroyByPassDtor(TObject addr, void *extra) {
    return destroy_backend(addr, extra, TRUE);
}

TBool     TObject_Exists(TMemMgr *memmgr, TObject addr) {
    return TMemMgr_Exists(memmgr, ((TObjMta*)addr) - 1);
}

TSize     TObject_SizeOf(TObject addr) {
    TObjMta *acm = ((TObjMta*)addr) - 1;
    return TMemMgr_SizeOf(acm->memmgr, acm) - sizeof(TObjMta);
}

TObject  TObject_Resize    (TObject addr, TSize newSize) {
    TObjMta *ometaOld = ((TObjMta*)addr) - 1;
    TObjMta *ometaNew = TMemMgr_Realloc(ometaOld->memmgr, ometaOld, sizeof(TObjMta) + newSize);

    onerror(ometaNew)
        throw_note(ExceptionTObjectResize, NULL, "Falha de realocaçao de metadados")

    return (ometaNew->addr = (TObject*)(ometaNew + 1));
}

TMemMgr *TObject_ManagerOf (TObject addr) {
    TObjMta *acm = ((TObjMta*)addr) - 1;
    return acm->memmgr;
}


