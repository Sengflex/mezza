#define MZ_STRINGBUILDER_IMPLEMENT
#include "StringBuilder.h"

#include <string.h>
#include <stdarg.h>

#include "../Lang.h"
#include "../OnError.h"
#include "../base/TObject.h"

#include "../FileUtil.h"

/* Faz alimpeza em caso de alocações parciais */
PRIVATE void FromString_aux_Clean ( va_list ap, TSize  howmanyvars ) {
    char **vchar;

    loop_i ( howmanyvars )
        vchar = va_arg ( ap, char** );
        TObject_Destroy ( *vchar, NULL );
    end_loop_i
}

#define FromString_aux_AllocOrClean(pmemlist, pNewStr, srcStr, newStrSz, ap, format, howmanyvars, firstStr) \
    onerror( *pNewStr = TString_Create(pmemlist, srcStr, newStrSz) ) { \
        va_end ( ap ); \
        va_start ( ap, format ); \
        FromString_aux_Clean ( ap, howmanyvars ); \
        va_end ( ap ); \
        throw_note(ExceptionStringBuilder, NULL, "Operacao FromString. Alocacao de string destino falhou") \
	} \
    if(!firstStr) \
        firstStr = *pNewStr; \
    (howmanyvars)++;

/* "name = Oulard"  "%=%" */
TString StringBuilder_FromString ( TMemMgr *pmemlist, char* src, char *format, ... ) {
    char *cur = src;
    TSize  /*size = 0,*/ howmanyvars = 0;
    va_list ap;
    TString firstStr = NULL;

    va_start ( ap, format );

    while ( *format ) {
        if ( *format == '%' ) {
            TString *newStr;
            char *end;
            TSize count;

            newStr = va_arg ( ap, TString* );
            format++;
            if ( *format == '\0' ) {
                FromString_aux_AllocOrClean(pmemlist, newStr, cur, 0, ap, format, howmanyvars, firstStr)

                //size += 1;
                break;
            }

            end = strchr ( cur, *format );
            onerror( end )
                throw_note(ExceptionStringBuilder, NULL, "Operacao FromString. Formato nao corresponde")

            count = ( end - cur ) / sizeof ( char );

            FromString_aux_AllocOrClean(pmemlist, newStr, NULL, count, ap, format, howmanyvars, firstStr)

            TString_Copy ( newStr, cur, count );
            cur = end + 1;
            //size += count;

        } else if ( *format == '*' ) {
            char *end;

            format++;
            if ( *format == '\0' ) {
                break;
            }

            end = strchr ( cur, *format );
            onerror( end )
                throw_note(ExceptionStringBuilder, NULL, "Operacao FromString. Formato nao corresponde")

            cur = end + 1;
        } else {
            char *pos;

            pos = strchr ( cur, *format );
            if ( !pos )
                break;
            cur = pos+1;
        }
        format++;
    }

    va_end ( ap );

    return firstStr;
}


TString StringBuilder_FromFile ( TMemMgr *pmemlist, TString *str, FILE *fp ) {
    TSize       fsize      = FileUtil_TailSizeOf ( fp );
    unsigned int  fposbackup = ftell ( fp );

    *str = TString_Create ( pmemlist, NULL, fsize );
    onerror( *str )
        throw_note(ExceptionStringBuilder, NULL, "Operacao FromFile. Falha em criar objeto conteiner TString")

    onerror( fsize == fread ( *str, sizeof ( char ), fsize, fp ) ) {
        TObject_Destroy (*str, NULL );
        fseek ( fp, fposbackup, SEEK_SET );
        throw_note(ExceptionStringBuilder, NULL, "Operacao FromFile. Falha em ler o arquivo")
    }

    ( *str ) [fsize] = '\0';

    return *str;
}

TString StringBuilder_FromFileSection ( TMemMgr *pmemlist, TString *str, FILE *fp, int sep ) {
    TSize      fsize = FileUtil_SectionSizeOf ( fp, sep );
    unsigned int fposbackup;

    *str = TString_Create ( pmemlist, NULL, fsize );
    onerror( *str )
        throw_note(ExceptionStringBuilder, NULL, "Operacao FromFile. Falha em criar objeto conteiner TString")

    fposbackup = ftell ( fp );
    onerror( fsize == fread ( *str, sizeof ( char ), fsize, fp ) ) {
        TObject_Destroy (*str, NULL );
        fseek ( fp, fposbackup, SEEK_SET );
        throw_note(ExceptionStringBuilder, NULL, "Operacao FromFile. Falha em ler o arquivo")
    }

    ( *str ) [fsize] = '\0';

    return *str;
}
