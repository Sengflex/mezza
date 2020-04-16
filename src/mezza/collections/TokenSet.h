
/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * collections/TokenSet.h
 *
 *  Created on: 21 de dez de 2018
 *      Author: Danilo Almeida
 */

#ifndef MZ_TokenSet_H
#define MZ_TokenSet_H

#include "../Type.h"
#include "../base/TMemMgr.h"
#include "../base/Exceptions.h"
#include "../base/TList.h"
#include "../str/TString.h"

#ifndef MZ_TokenSet_IMPLEMENT
    declare_exception(ExceptionTTokenSetCreation)
    declare_exception(ExceptionTTokenSetAddition)
    declare_exception(ExceptionTTokenSetRemotion)
    declare_exception(ExceptionTTokenSetNotFoundToken)
    declare_exception(ExceptionTTokenSetTokenSetting)
#else
    define_exception(ExceptionTTokenSetCreation, "Falha em criar o objeto TList")
    define_exception(ExceptionTTokenSetAddition, "Falha em adicionar novo token")
    define_exception(ExceptionTTokenSetRemotion, "Falha em remover token")
    define_exception(ExceptionTTokenSetNotFoundToken, "Token nao encontrado")
    define_exception(ExceptionTTokenSetTokenSetting, "Falha em mudar o valor do token")
#endif

int token_tlistcallback(TNode *node, void *tkntext);

/**
 *
 * Dispara: ExceptionTTokenSetCreation
 * */
TList     *TokenSet_CreateFromString(TMemMgr *memmgr, char *source, char *seps);
/**
 *
 * @return O token adicionado. Será, naturalmente, uma strng de endereço
 * diferente daquele do parâmetro 'token'
 *
 * Dispara: ExceptionTTokenSetAddition
 * */
TString    TokenSet_Add(TList *tokenset, char *token);

#define    TokenSet_AddPreCreated(TKNSET, TKN) TList_AddObj(TKNSET, TKN)

#define    TokenSet_Get(TKNSET, TKN) TList_Find(TKNSET, token_tlistcallback, TKN)
/**
 *
 * Dispara: ExceptionToString
 * */
TString    TokenSet_ToString(TList *tokenset, int charSep);


#endif /* MZ_TokenSet_H */
