
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


#include "TMemMgr.h"
#include "../OnError.h"
#include "../Type.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TMemMgr  *TMemMgr_Init(TMemMgr *memmgr) {
    if(!memmgr) {
        memmgr = malloc(sizeof(TMemMgr));
        onerror(memmgr) {
            fputs("libmezza.base.mem.ZMemInit(1): Alocação de lista. Malloc falhou\n", stderr);
            return NULL;
        }
    }
    memmgr->end = memmgr->start = NULL;
    memmgr->nofAlocated = 0;
    return memmgr;
}

void      *TMemMgr_Alloc(TMemMgr *memlist, TSize size) {
    /* Alocação de novo nó */
    TMemNod *node = malloc(sizeof(TMemNod));

    onerror(node) {
        fputs("libmezza.base.mem.AllocMemory(1): Alocação de nó da lista. Malloc falhou\n", stderr);
        return NULL;
    }

    /* Alocação de novo item de memória no nó */
    node->item = malloc(sizeof(TMemMta) + size);

    onerror(node->item) {
        fputs("libmezza.base.mem.AllocMemory(2): Alocação de item. Malloc falhou\n", stderr);
        free(node);
        return NULL;
    }

    node->item->addr = (void *)(node->item + 1);
    node->item->itemSizeInBytes = size;

    node->next = NULL;
    node->prev = memlist->end;

    if(!memlist->start)
        memlist->start = node;

    if(memlist->end)
        memlist->end->next = node;
    memlist->end = node;
    memlist->nofAlocated++;

    return node->item->addr;
}

static TMemNod *GetNodeForAddr(TMemMgr *memlist, void * addr) {
    TMemNod *cursor = memlist->start;

    while(cursor) {
        if(cursor->item->addr == addr)
            return cursor;

        cursor = cursor->next;
    }

    return NULL;
}

TBool     TMemMgr_Exists(TMemMgr *memlist, void * addr) {
    if(GetNodeForAddr(memlist, addr))
        return TRUE;

    return FALSE;
}

TSize     TMemMgr_SizeOf(TMemMgr *memlist, void * addr) {
    TMemNod *node = GetNodeForAddr(memlist, addr);

    if(node)
        return node->item->itemSizeInBytes;

    return 0;
}

void      *TMemMgr_Realloc(TMemMgr *memmgr, void *addr, TSize size) {
    TMemNod *node = GetNodeForAddr(memmgr, addr);
    void     *ret = NULL;

        if(node) {
            TMemMta *newMeta = realloc(node->item, sizeof(TMemMta)+size);
            onerror(newMeta) {
                fputs("libmezza.base.mem.ZMemRealloc: falha de realocação de item\n",
                      stderr);
                return NULL;
            }

            node->item = newMeta;
            newMeta->addr = (void*)(newMeta + 1);
            newMeta->itemSizeInBytes = size;

            ret = newMeta->addr;
        }
    return ret;
}

void * TMemMgr_Free(TMemMgr *memmgr, void * addr) {
    TMemNod *node = GetNodeForAddr(memmgr, addr);

    if(node) {

        if(node->prev)
            node->prev->next = node->next;
        if(node->next)
            node->next->prev = node->prev;

        if(memmgr->start == node)
            memmgr->start = node->next;

        if(memmgr->end == node)
            memmgr->end = node->prev;

        memmgr->nofAlocated--;

        free(node->item);
        free(node);
    }

    return NULL;
}

void      TMemMgr_FreeAll(TMemMgr *memmgr) {
    if(memmgr) {
        TMemNod *cursor = memmgr->start;
        TMemNod *toFreeNode;

        while(cursor) {
            toFreeNode = cursor;
            cursor = cursor->next;
            free(toFreeNode->item);
            free(toFreeNode);
        }

        TMemMgr_Init(memmgr);
    }
}

TMemString TMemMgr_CreateString(TMemMgr *memmgr, char *cstr, TSize cstrSz) {
    TMemString ret;
    TSize sizeToAlloc = cstrSz;
	TSize cstrLen=0;

    if(cstr) {
        cstrLen = strlen(cstr);

        if(sizeToAlloc < cstrLen)
            sizeToAlloc = cstrLen;
    }
    sizeToAlloc++;


    ret = TMemMgr_Alloc(memmgr, sizeToAlloc * sizeof(char));
    onerror(ret) {
        fputs("libmezza.base.mem.AllocMemory(1): Alocação de nó da lista. Malloc falhou\n", stderr);
    }

    if(cstr) strcpy(ret, cstr);

    return ret;
}
