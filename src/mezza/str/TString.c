
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

#define MZ_STR_TSTRING_H_IMPLEMENT
#include "TString.h"

#include <stdarg.h>
#include <string.h>

#include "../OnError.h"

#include "../base/TObject.h"
#include "../str/StringUtils.h"

TString TString_Create(TMemMgr *memmgr, char *cstr, TSize cstrSz) {
    TString string_ret;
    TCount  howManyCharsToAllocate;

    if(cstr) {
        howManyCharsToAllocate = strlen(cstr);
        if(howManyCharsToAllocate < cstrSz) howManyCharsToAllocate = cstrSz;
    } else {
        howManyCharsToAllocate = cstrSz;
    }
    howManyCharsToAllocate++;

    string_ret = TObject_Create(memmgr, sizeof(char) * howManyCharsToAllocate, NULL);
    onerror(string_ret)
		throw(ExceptionTStringCreation, NULL)

    if(cstr) {
        strcpy(string_ret, cstr);
    } else {
        *string_ret = 0;
    }

    return string_ret;
}

TString TString_Copy(TString *dest, char *orig, TSize origSz) {
    TSize howMuchCopy = strlen(orig);

    /* Checa se é para copiar menos que o número
     * de caracteres na string de origem */
    if(origSz && (origSz < howMuchCopy)) howMuchCopy = origSz;

    /* Redimensiona o objeto String se necessário */
    if(howMuchCopy > TString_SizeOf(*dest)) {
		onerror(TString_Resize(dest, howMuchCopy))
			throw(ExceptionTStringCopy, NULL)
    }

    /* Faz a cópia */
    strncpy(*dest, orig, howMuchCopy);
    (*dest)[howMuchCopy] = 0;

    return *dest;
}

TString TString_CopyF(TString *dest, char *format, ...){
    va_list ap;
    TSize   neededSize;

    /* Calcula tamanho necessário */
    va_start ( ap, format );
    neededSize = StringUtils_VCalcSizeF ( format, ap );
    va_end ( ap );

    /* Redimensiona se necessário */
    if ( neededSize > TString_SizeOf ( *dest ) ) {
		onerror(TString_Resize(dest, neededSize))
			throw_note(ExceptionTStringCopy, NULL, "Operacao de copia com formato")
    }

    /* Imprime com formatação no objeto String */
    va_start ( ap, format );
    vsprintf ( *dest, format, ap );
    va_end ( ap );

    return *dest;
}

TString TString_Concat(TString *dest, char *orig) {
    TSize neededSize;

    neededSize = strlen(*dest) + strlen(orig);

    if(neededSize > TString_SizeOf(*dest)) {
		onerror(TString_Resize(dest, neededSize))
			throw(ExceptionTStringConcat, NULL)
    }

    strcat(*dest, orig);

    return *dest;
}

TString TString_Resize(TString *dest, TSize newSize) {
    TString string_ret;

    string_ret = TObject_Resize(*dest, sizeof(char) * (newSize+1));
	onerror(string_ret)
		throw(ExceptionTStringResize, NULL)

    string_ret[newSize] = 0;

    *dest = string_ret;

    return string_ret;
}

TSize   TString_SizeOf(TString str) {
    return (TObject_SizeOf(str)/sizeof(char)) - 1;
}

TString TString_ConcatF( TString *dest,  char *format, ...) {
    va_list ap;
    TSize   neededSize, destLen = strlen(*dest);

    /* Calcula o tamanho necessario */
    va_start(ap, format);
    neededSize = destLen + StringUtils_VCalcSizeF(format, ap);
    va_end(ap);

    /* Redimensiona se preciso */
    if(neededSize > TString_SizeOf(*dest)) {
		onerror(TString_Resize(dest, neededSize))
			throw_note(ExceptionTStringConcat, NULL, "Operacao de concatenacao com formato")
    }

    /* Concatena com formato */
    {
        char *copyTo = *dest;

        va_start(ap, format);
        vsprintf(&copyTo[destLen], format, ap);
        va_end(ap);
    }

    return *dest;
}
