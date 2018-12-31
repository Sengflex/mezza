
#define MZ_FILE_H_IMPLEMENT
#define MZ_FILE_IMPL
#include "FileUtil.h"

#include <stdio.h>
#include <string.h>

#include "OnError.h"

TSize FileUtil_SizeOf( FILE *fp ) {
    TPosition fpos_bkp = ftell(fp);
    TSize ret;

    fseek(fp, 0, SEEK_END);
    ret = ftell(fp);
    fseek(fp, fpos_bkp, SEEK_SET);

    return ret;
}

TSize FileUtil_TailSizeOf(FILE *fp) {
    TPosition filepos_bkp = ftell(fp);
    TSize ret;

    fseek(fp, 0, SEEK_END);
    ret = ftell(fp) - filepos_bkp;
    fseek(fp, filepos_bkp, SEEK_SET);

    return ret;
}

TSize FileUtil_SectionSizeOf( FILE *fp, int sep) {
    TPosition fsize_bkp = ftell(fp);
    TCount read_bytes;
    TSize ret;
    char c;

    do {
		read_bytes=fread(&c, 1, sizeof(char), fp);
		onerror(read_bytes > 0) {
			fseek(fp, fsize_bkp, SEEK_SET);
			throw_note(ExceptionFIleIOUnexpectedEOF, 0, "Operacao SectionSizeOf")
		}
    } while(c!=(char)sep);

    ret = ftell(fp) - fsize_bkp - sizeof(char);
    fseek(fp, fsize_bkp, SEEK_SET);

    return ret;
}

TPosition FileUtil_PositionAtChar(FILE *fp, int c) {
    TPosition fsize_bkp = ftell(fp), ret;
    TCount read_bytes;
    char read_c;

    do {
		read_bytes=fread(&read_c, 1, sizeof(char), fp);
		onerror(read_bytes > 0) {
			fseek(fp, fsize_bkp, SEEK_SET);
			throw_note(ExceptionFIleIOUnexpectedEOF, 0, "Operacao PositionAtChar")
		}
    } while(read_c!=(char)c);

    fseek(fp, -1, SEEK_CUR);
    ret = ftell(fp);

    return ret;
}

TPosition FileUtil_PositionOf( FILE *fp, int c) {
    TSize fsize_bkp = ftell(fp), read_bytes;
    TPosition ret;
    char read_c;

    do {
		read_bytes=fread(&read_c, 1, sizeof(char), fp);
		onerror(read_bytes > 0) {
			fseek(fp, fsize_bkp, SEEK_SET);
			throw_note(ExceptionFIleIOUnexpectedEOF, 0, "Operacao PositionOf")
		}
    } while(read_c!=(char)c);

    ret = ftell(fp) - sizeof(char);

    fseek(fp, fsize_bkp, SEEK_SET);

    return ret;
}

TCount FileUtil_SkipChars(FILE *fp, char *c) {
    TPosition fpos_bkp = ftell(fp);
    TCount count=0, read_bytes;
    char read_c;

    do {
		read_bytes=fread(&read_c, 1, sizeof(char), fp);
		onerror(read_bytes > 0) {
			fseek(fp, fpos_bkp, SEEK_SET);
			throw_note(ExceptionFIleIOUnexpectedEOF, 0, "Operacao SkipChars")
		}
        if(strchr(c, read_c))
            count++;
        else
            break;
    } while(1);

    fseek(fp, -1, SEEK_CUR);

    return count;
}
