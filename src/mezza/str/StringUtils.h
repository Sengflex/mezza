
/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * str/StringUtils.h
 *
 *  Created on: before v0.2.0
 *      Author: danilo
 */

#ifndef MZ_STRINGUTILS_H
#define MZ_STRINGUTILS_H

#include <stdarg.h>

#include "../Type.h"

int     StringUtils_CompareF(char *src, char *format, ...);

char   *StringUtils_TruncRevSkip(char *str, int c, TCount skip);

char   *StringUtils_TruncRev(char *str, int c);

TSize   StringUtils_CalcSizeF(char *format, ...);

TSize   StringUtils_VCalcSizeF( char *format, va_list ap) ;

char   *StringUtils_Trimm(char *str, int c);

char   *StringUtils_TrimmAll(char *str, char *all);

#endif /* MZ_STRINGUTILS_H */
