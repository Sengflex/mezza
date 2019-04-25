/*
 * list.c
 *
 *  Created on: 9 de nov de 2018
 *      Author: danilo
 */

#define MZ_BASE_LIST_H_IMPLEMENT
#include "TList.h"

#include "TObject.h"
#include "Exceptions.h"
#include "TMemMgr.h"
#include "../OnError.h"

#include <stdio.h>

static void fAcmCleanerList(TObject addr, void *ignore) {
    TList *list = (TList *)addr;
    TLstNod *cursor = list->start;
    TLstNod *nodeToClean;

    while(cursor) {
        nodeToClean = cursor;
        cursor = cursor->next;
        TObject_Destroy(nodeToClean, (void *)TRUE);
    }
}

static void fAcmCleanerListNode(TObject addr, void *shouldIgnoreCleaning) {
    if(!shouldIgnoreCleaning) {
        TLstNod *lnode = (TLstNod *)addr;

        if(lnode->prev)
            lnode->prev->next = lnode->next;
        if(lnode->next)
            lnode->next->prev = lnode->prev;
    }
}

TList    *TList_Create(TMemMgr *memlist) {
    TList *list = TObject_Create(memlist, sizeof(TList), fAcmCleanerList);

    onerror(list)
        throw(ExceptionTListCreation, NULL)

    list->start = list->end = NULL;
    list->size  = 0L;
    return list;
}

TItem TList_Add(TList *list, void *item) {
    TLstNod *lnode = TObject_Create(TObject_ManagerOf(list), sizeof(TLstNod), fAcmCleanerListNode);

    onerror(lnode)
        throw_note(ExceptionTListAddition, NULL, "Alocação de nó para a lista")

    lnode->next     = NULL;
    lnode->prev     = list->end;
    lnode->item     = item;
    if(list->end)
        list->end->next = lnode;

    list->end       = lnode;
    if(!list->start)
        list->start = lnode;

    list->size++;

    return item;
}

TLstNod *TList_Find(TList *list, FListNodeCallback finder, void *extra) {
    TLstNod *cursor = list->start;

    while(cursor) {
        if(finder(cursor, extra))
            break;
        cursor = cursor->next;
    }
    return cursor;
}

TLstNod *TList_Get(TList *list, void *item) {
    TLstNod *cursor = list->start;

    while(cursor) {
        if(cursor->item == item)
            break;
        cursor = cursor->next;
    }
    return cursor;
}

TLstNod     *TList_Rem(TList *list, TLstNod *node) {
    TLstNod *ret = NULL;
    if(node) {
        if(list->start == node)
            list->start = node->next;

        if(list->end == node)
            list->end = node->prev;
        ret = node->next;
        TObject_Destroy(node, NULL);

        list->size--;
    }
    return ret;
}

TLstNod *TList_RemTObject    (TList *list, TLstNod *node) {
	TObject_Destroy(node->item, NULL);
	return TList_Rem(list, node);
}

int        TList_Foreach(TList *list, FListNodeCallback cback, void *extra) {
    TLstNod *cursor = list->start;
    int ret=0;
    while(cursor) {
        if((ret=cback(cursor, extra)))
            break;
        cursor = cursor->next;
    }
    return ret;
}

void TList_ForeachDoDestroy(TList *list, void *extra) {
    TLstNod *cursor = list->start;

    while(cursor) {
    	cursor->item = TObject_Destroy(cursor->item, extra);
        cursor = cursor->next;
    }
}

void TList_ForeachDoDestroyByPassDtor(TList *list) {
    TLstNod *cursor = list->start;

    while(cursor) {
    	cursor->item = TObject_DestroyByPassDtor(cursor->item, NULL);
        cursor = cursor->next;
    }
}

TLstNod* TList_GetAt(TList* list, TPosition pos) {
    TLstNod* cursor = list->start;
    TCount counter = 0;
    while (counter < pos) {
        if (!cursor)
            break;

        cursor = cursor->next;
        counter++;
    }
    return cursor;
}

TSize    TList_CountNodes(TList *list) {
    TSize ret = 0;

    LOOPLIST(list, ret++;) END_LOOPLIST

    return ret;
}

