
/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * base/TMemMgr.h
 *
 *  Created on:  9 de nov de 2018
 *      Author: Danilo Almeida
 */

#ifndef BASE_MEM_MANAGER_H_
#define BASE_MEM_MANAGER_H_

#include "../Type.h"

typedef struct MemMgr  TMemMgr;
typedef struct MemNod  TMemNod;
typedef struct MemMta  TMemMta;
typedef char *TMemString;

struct MemMta {
    void       *addr;
    TSize       itemSizeInBytes;
};

struct MemNod {
    TMemMta *item;
    TMemNod *next;
    TMemNod *prev;
};

struct MemMgr {
    TMemNod *start;
    TMemNod *end;
    TCount       nofAlocated;
};

TMemMgr  *TMemMgr_Init(TMemMgr *memManager);

void     *TMemMgr_Alloc(TMemMgr *memManager, TSize size);

void     *TMemMgr_Realloc(TMemMgr *memManager, void *ptr, TSize size);

void     *TMemMgr_Free(TMemMgr *memManager, void *addr);

void      TMemMgr_FreeAll(TMemMgr *memManager);

TBool     TMemMgr_Exists(TMemMgr *memManager, void *addr);

TSize     TMemMgr_SizeOf(TMemMgr *memManager, void *addr);

TMemString TMemMgr_CreateString(TMemMgr *memManager, char *cstr, TSize cstrSz);

#endif /* BASE_MEM_MANAGER_H_ */
