
/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * SimpleTest.h
 *
 *  Created on: before v.2.0
 *      Author: danilo
 */

#ifndef MZ_FILE_H
#define MZ_FILE_H

#include "Lang.h"
#include "Type.h"
#include <stdio.h>

#include "base/Exceptions.h"

#ifndef MZ_FILE_H_IMPLEMENT
    declare_exception(ExceptionFIleIOUnexpectedEOF)
    declare_exception(ExceptionFIleOpening)
#else
    define_exception(ExceptionFIleIOUnexpectedEOF, "Fim de arquivo inesperado")
    define_exception(ExceptionFIleOpening, "Falha em abrir arquivo")
#endif

TSize     FileUtil_SizeOf( FILE *fp );

TSize     FileUtil_TailSizeOf(FILE *fp);
/**
 *
 * Dispara: ExceptionFIleIOUnexpectedEOF
 * */
TSize     FileUtil_SectionSizeOf( FILE *fp, int sep);
/**
 *
 * Dispara: ExceptionFIleIOUnexpectedEOF
 * */
TPosition FileUtil_PositionOf( FILE *fp, int c);
/**
 *
 * Dispara: ExceptionFIleIOUnexpectedEOF
 * */
TCount    FileUtil_SkipChars(FILE *fp, char *c);
/**
 *
 * Dispara: ExceptionFIleIOUnexpectedEOF
 * */
TPosition FileUtil_PositionAtChar(FILE *fp, int c);

#endif /* MZ_FILE_H */
