
/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * str/TString.h
 *
 *  Created on: 12 de nov de 2018
 *      Author: danilo
 */

#ifndef MZ_TSTRING_H
#define MZ_TSTRING_H

#include "../base/TMemMgr.h"
#include "../base/Exceptions.h"

typedef char *TString;

#ifndef MZ_STR_TSTRING_H_IMPLEMENT
    declare_exception(ExceptionTStringCreation)
	declare_exception(ExceptionTStringCopy)
	declare_exception(ExceptionTStringConcat)
	declare_exception(ExceptionTStringResize)
	declare_exception(ExceptionToString)
#else
    define_exception(ExceptionTStringCreation, "Falha em criar a string")
    define_exception(ExceptionTStringCopy, "Falha em copiar para a string")
    define_exception(ExceptionTStringConcat, "Falha em concatenar a string")
    define_exception(ExceptionTStringResize, "Falha em redimensionar a string")
    define_exception(ExceptionToString, "Falha em converter dados em string")
#endif

/**
 *
 * Dispara: ExceptionTStringCreation
 * */
TString TString_Create(TMemMgr *memmgr, char *cstr, TSize cstrSz);
/**
 *
 * Dispara: ExceptionTStringCopy
 * */
TString TString_Copy(TString *dest, char *orig, TSize origSz);
/**
 *
 * Dispara: ExceptionTStringCopy
 * */
TString TString_CopyF(TString *dest, char *format, ...);
/**
 *
 * Dispara: ExceptionTStringConcat
 * */
TString TString_Concat(TString *dest, char *orig);
/**
 *
 * Dispara: ExceptionTStringConcat
 * */
TString TString_ConcatF(TString *dest, char *format, ...);
/**
 *
 * Dispara: ExceptionTStringResize
 * */
TString TString_Resize(TString *dest, TSize newSize) ;

TSize   TString_SizeOf(TString str);

#endif /* MZ_TSTRING_H */
