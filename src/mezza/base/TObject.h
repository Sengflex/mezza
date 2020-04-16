
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

#ifndef MZ_BASE_OBJECT_H
#define MZ_BASE_OBJECT_H

#include "TMemMgr.h"
#include <stdio.h>
#include <stdlib.h>
#include "Exceptions.h"

typedef void  *TObject;
typedef void (*FObjDestructor)(TObject, void*);

#ifndef MZ_BASE_OBJECT_H_IMPLEMENT
    declare_exception(ExceptionTObjectCreation)
    declare_exception(ExceptionTObjectResize)
#else
    define_exception(ExceptionTObjectCreation, "Falha em criar o objeto")
    define_exception(ExceptionTObjectResize, "Falha em redimensionar o objeto")
#endif

/**
 *
 * Dispara: ExceptionTObjectCreation
 * */
TObject  TObject_Create (TMemMgr *memmgr, TSize sizeInBytes, FObjDestructor cleaner);
/**
 *
 * Dispara: ExceptionTObjectCreation
 * */
TObject  TObject_CreateZero(TMemMgr *memmgr, TSize sizeInBytes, FObjDestructor cleaner);

TObject  TObject_SetDtor   (TObject self, FObjDestructor dtor);

TObject  TObject_Destroy   (TObject addr, void *extra);

TObject  TObject_DestroyByPassDtor(TObject addr, void *extra);
/**
 *
 * Dispara: ExceptionTObjectResize
 * */
TObject  TObject_Resize    (TObject addr, TSize newSize);

TBool    TObject_Exists    (TMemMgr *memmgr, TObject addr);

TSize    TObject_SizeOf    (TObject addr);

TMemMgr *TObject_ManagerOf (TObject addr);

#endif /* MZ_BASE_OBJECT_H */
