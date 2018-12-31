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
 * Throws: ExceptionFIleIOUnexpectedEOF
 * */
TSize     FileUtil_SectionSizeOf( FILE *fp, int sep);
/**
 *
 * Throws: ExceptionFIleIOUnexpectedEOF
 * */
TPosition FileUtil_PositionOf( FILE *fp, int c);
/**
 *
 * Throws: ExceptionFIleIOUnexpectedEOF
 * */
TCount    FileUtil_SkipChars(FILE *fp, char *c);
/**
 *
 * Throws: ExceptionFIleIOUnexpectedEOF
 * */
TPosition FileUtil_PositionAtChar(FILE *fp, int c);

#endif /* MZ_FILE_H */
