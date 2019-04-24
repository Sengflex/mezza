/*
 * TList.h
 *
 *  Created on: 10 de nov de 2018
 *      Author: danilo
 */

#ifndef BASE_LIST_H_
#define BASE_LIST_H_

/*
 * list.c
 *
 *  Created on: 9 de nov de 2018
 *      Author: danilo
 */


#include "Exceptions.h"
#include "TMemMgr.h"
#include "../OnError.h"

#include <stdio.h>

typedef struct List      TList;
typedef struct LstNod   TLstNod;
typedef void            *TItem;

typedef int (*FListNodeCallback)(TLstNod *, void *);

#ifndef MZ_BASE_LIST_H_IMPLEMENT
    declare_exception(ExceptionTListCreation)
    declare_exception(ExceptionTListAddition)
#else
    define_exception(ExceptionTListCreation, "Falha em criar a lista")
    define_exception(ExceptionTListAddition, "Falha em adicionar item à lista")
#endif

struct LstNod {
    void    *item;
    TLstNod *prev;
    TLstNod *next;
};

struct List {
    TLstNod *start;
    TLstNod *end;
};
/**
 *
 * Dispara: ExceptionTListCreation
 * */
TList   *TList_Create(TMemMgr *memmgr);

/**
 *
 * Dispara: ExceptionTListAddition
 * */
TItem    TList_Add    (TList *list, void *item);

TLstNod *TList_Get    (TList *list, void *item);

TLstNod *TList_GetAt  (TList *list, TPosition pos);

TLstNod *TList_Find   (TList *list, FListNodeCallback finder, void *extra);

TLstNod *TList_Rem    (TList *list, TLstNod *node);

TLstNod *TList_RemTObject    (TList *list, TLstNod *node);

int      TList_Foreach(TList *list, FListNodeCallback cback, void *extra);

void     TList_ForeachDoDestroy(TList *list, void *extra);

void     TList_ForeachDoDestroyByPassDtor(TList *list);

TSize    TList_CountNodes(TList *list);

#define LOOPLIST(LIST, ACTION) { \
        TLstNod *_NODE_ = (LIST)->start; \
        while(_NODE_) { \
            ACTION

#define CONTINUE_LOOP_LIST \
        _NODE_ = _NODE_->next; \
        continue;

#define END_LOOPLIST  \
            _NODE_ = _NODE_->next;   \
        }  \
    }


#endif /* BASE_LIST_H_ */
