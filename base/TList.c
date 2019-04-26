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

static void TList_dtor(TObject obj, void *userdata) {
    TLstNod *cursor = ((TList *)obj)->start;
    TLstNod *nodeToClean;

    while(cursor) {
        nodeToClean = cursor;
        cursor = cursor->next;
        TObject_Destroy(nodeToClean, userdata);
    }
}

static void TListNode_dtor(TObject addr, void *userdata) {
    TLstNod *lnode = (TLstNod *)addr;

    if(lnode->itemIsObj == TRUE)
        TObject_Destroy(lnode->item, userdata);
}

TList    *TList_Create(TMemMgr *memlist) {
    TList *list = TObject_Create(memlist, sizeof(TList), TList_dtor);

    onerror(list)
        throw(ExceptionTListCreation, NULL)

    list->start = list->end = NULL;
    list->size  = 0L;
    return list;
}

TItem    TList_Add__Backend    (TList *list, void *item, TBool itemIsObj) {
    TLstNod *lnode = TObject_Create(TObject_ManagerOf(list), sizeof(TLstNod), TListNode_dtor);

    onerror(lnode)
        throw_note(ExceptionTListAddition, NULL, "Alocação de nó para a lista")

    lnode->next     = NULL;
    lnode->prev     = list->end;
    lnode->item     = item;
    lnode->itemIsObj = itemIsObj;
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

TBool TList_CheckNode(TList *list, TLstNod *node) {
    TLstNod *cur;

    LOOPLIST(list, )
        if(_NODE_ == node)
            return TRUE;
    END_LOOPLIST

    return FALSE;
}

TStatus      TList_Unlink(TList *list, TLstNod *node) {
    if(TList_CheckNode(list, node)) {
        if(node->prev)
            node->prev->next = node->next;
        if(node->next)
            node->next->prev = node->prev;
        if(list->start == node)
            list->start = node->next;
        if(list->end == node)
            list->end = node->prev;
        list->size--;
        return OK;
    }
    return FAIL;
}

TLstNod     *TList_Rem__Backend(TList *list, TLstNod *node, void *userdata) {
    TLstNod *ret = NULL;

    if(node && (TList_Unlink(list, node) == OK)) {
        ret = node->next;
        TObject_Destroy(node, userdata);
    }
    return ret;
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

void TList_ForeachDoDestroy(TList *list, void *userdata) {
    TLstNod *cursor = list->start;

    while(cursor) {
    	cursor->item = TObject_Destroy(cursor->item, userdata);
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

