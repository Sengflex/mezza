
#include "StringUtils.h"

#include <stdio.h>
#include <string.h>

#include "../Lang.h"

char *StringUtils_TruncRev(char* str, int c) {
    return StringUtils_TruncRevSkip(str, c, 0);
}

char *StringUtils_TruncRevSkip(char *srcstr, int sepchar, TCount skipcount) {
    /* Percorre a string fonte caractere a caractere */
    LOOP_INDEX_REVERSE(strlen(srcstr))
        /* Se encontrar o separador */
        if(srcstr[_INDEX_]==(char)sepchar)
        {
            /* Se é para saltá-lo esta vez... */
            if(skipcount>0) {
                skipcount--; /* Atualiza o contador de saltos */
            } else { /* .. senão é para saltar */
                /* Retorna a substring que inicia no
                 * caractere seguinte */
                return &srcstr[_INDEX_+1];
            }
        }
    LOOP_INDEX_REVERSE_END

    return NULL;
}

TSize StringUtils_CalcSizeF(char *format, ...) {
    TSize size;
    va_list ap;

    va_start(ap, format);

    size = StringUtils_VCalcSizeF(format, ap);

    va_end(ap);

    return size;
}

char* StringUtils_Trimm(char* str, int c) {
    char *cur = str;
    char *dest;
    TSize times;

    while(*cur && *cur==(char)c)
        cur++;

    if(!*cur) {
        *str=0;
        return str;
    }
    dest = str;
    while(*cur) {
        *dest = *cur;
        dest++;
        cur++;
    }
    *dest = *cur;
    times = strlen(str);
    cur = str;
    LOOP_INDEX_REVERSE(times)
        if(cur[_INDEX_]!=(char)c) {
            cur[_INDEX_+1]=0;
            break;
        }
    LOOP_INDEX_REVERSE_END

    return str;
}

char* StringUtils_TrimmAll(char* str, char *all) {
    char *cur = str;
    char *dest;
    TSize times;

    while(*cur && strchr(all, *cur)) {
        cur++;
    }
    if(!*cur) {
        *str=0;
        return str;
    }
    dest = str;
    while(*cur) {
        *dest = *cur;
        dest++;
        cur++;
    }
    *dest = *cur;
    times = strlen(str);
    cur = str;
    LOOP_INDEX_REVERSE(times)
        if(!strchr(all, cur[_INDEX_])) {
            cur[_INDEX_+1]=0;
            break;
        }
    LOOP_INDEX_REVERSE_END

    return str;
}

int StringUtils_CompareF( char *src,  char *format, ...) {
    char diff;
    union {
        char c;
        char *s;
        int i;
        unsigned int u;
    } u;
    char number[50];
    va_list ap;
    int res = 0;

    va_start(ap, format);

    for(; *format; ++format, ++src)
    {
        if(*format=='%')
        {
            /* Processa par?metro conforme o fomato */
            ++format;
            switch(*format) {
            case 'c': {
                    u.c = va_arg(ap, int);
                    diff = *src - u.c;
                    if(diff) {
                        if(diff>0) {
                            res = 1;
                            goto retpoint;
                        } else {
                            res = -1;
                            goto retpoint;
                        }
                    }
                }
                break;
            case 's': {
                    TSize len;
                    u.s = va_arg(ap, char*);
                    len = strlen(u.s);
                    if(strlen(src)>=len) {
                        res = strncmp(src, u.s, len);
                        if(res) {
                            goto retpoint;
                        }
                        src+=len-1;
                    } else {
                        res = -1;
                        goto retpoint;
                    }
                }
                break;
            case 'd': {
                    TSize len;
                    u.i = va_arg(ap, int);
                    sprintf(number, "%d", u.i);
                    len = strlen(number);
                    if(strlen(src)>=len) {
                        res = strncmp(src, number, len);
                        if(res) {
                            goto retpoint;
                        }
                        src+=len-1;
                    } else {
                        res = -1;
                        goto retpoint;
                    }
                }
                break;
            case 'u': {
                    TSize len;
                    u.u = va_arg(ap, unsigned int);
                    sprintf(number, "%u", u.u);
                    len = strlen(number);
                    if(strlen(src)>=len) {
                        res = strncmp(src, number, len);
                        if(res) {
                            goto retpoint;
                        }
                        src+=len-1;
                    } else {
                        res = -1;
                        goto retpoint;
                    }
                }
                break;
            case 'x': {
                    TSize len;
                    u.u = va_arg(ap, unsigned int);
                    sprintf(number, "%x", u.u);
                    len = strlen(number);
                    if(strlen(src)>=len) {
                        res = strncmp(src, number, len);
                        if(res) {
                            goto retpoint;
                        }
                        src+=len-1;
                    } else {
                        res = -1;
                        goto retpoint;
                    }
                }
                break;
            case 'X': {
                    TSize len;
                    u.u = va_arg(ap, unsigned int);
                    sprintf(number, "%X", u.u);
                    len = strlen(number);
                    if(strlen(src)>=len) {
                        res = strncmp(src, number, len);
                        if(res) {
                            goto retpoint;
                        }
                        src+=len-1;
                    } else {
                        res = -1;
                        goto retpoint;
                    }
                }
                break;
            case '%':
                goto blackhole;
                break;
            default:
                break;
            }
        }
        else
        {
blackhole:
            diff = *src - *format;
            if(diff) {
                if(diff>0) {
                    res = 1;
                    goto retpoint;
                } else {
                    res = -1;
                    goto retpoint;
                }
            }
        }
    }

    retpoint:
    va_end(ap);

    return res;
}




TSize StringUtils_VCalcSizeF( char *format, va_list ap) {
    TSize size=0;
    for(; *format; ++format)
    {
        if(*format=='%')
        {
            /* Processa par?metro conforme o fomato */
            ++format;
            if(*format=='%')
                goto blackhole;
            switch(*format)
            {
            case 'c':
                va_arg(ap, int);
                goto blackhole;
                break;
            case 's':
            {
                size += strlen(va_arg(ap, char*));
                break;
            }
            case 'd':
                va_arg(ap, int);
                size += 15;
                break;
            case 'f':
                va_arg(ap, double);
                size += 35;
                break;
            case 'u':
            case 'x':
            case 'X':
                va_arg(ap, unsigned int);
                size += 15;
                break;
            default:
                break;
            }
        }
        else
        {
            /* Adiciona o caractere */
blackhole:
            size++;
        }
    }

    return size;
}
