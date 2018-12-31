/*
 * TString.h
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
 * Throws: ExceptionTStringCreation
 * */
TString TString_Create(TMemMgr *memmgr, char *cstr, TSize cstrSz);
/**
 *
 * Throws: ExceptionTStringCopy
 * */
TString TString_Copy(TString *dest, char *orig, TSize origSz);
/**
 *
 * Throws: ExceptionTStringCopy
 * */
TString TString_CopyF(TString *dest, char *format, ...);
/**
 *
 * Throws: ExceptionTStringConcat
 * */
TString TString_Concat(TString *dest, char *orig);
/**
 *
 * Throws: ExceptionTStringConcat
 * */
TString TString_ConcatF(TString *dest, char *format, ...);
/**
 *
 * Throws: ExceptionTStringResize
 * */
TString TString_Resize(TString *dest, TSize newSize) ;

TSize   TString_SizeOf(TString str);

#endif /* MZ_TSTRING_H */
