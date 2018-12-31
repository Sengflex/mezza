/*
 * TTokenSet.h
 *
 *  Created on: 21 de dez de 2018
 *      Author: Danilo Almeida
 */

#ifndef MZ_TTOKENSET_H
#define MZ_TTOKENSET_H

#include "../Type.h"
#include "../base/TMemMgr.h"
#include "../base/Exceptions.h"
#include "../base/TList.h"
#include "../str/TString.h"

#ifndef MZ_TTOKENSET_IMPLEMENT
    declare_exception(ExceptionTTokenSetCreation)
    declare_exception(ExceptionTTokenSetAddition)
    declare_exception(ExceptionTTokenSetRemotion)
    declare_exception(ExceptionTTokenSetNotFoundToken)
    declare_exception(ExceptionTTokenSetTokenSetting)
#else
    define_exception(ExceptionTTokenSetCreation, "Falha em criar o objeto TTokenSet")
    define_exception(ExceptionTTokenSetAddition, "Falha em adicionar novo token")
    define_exception(ExceptionTTokenSetRemotion, "Falha em remover token")
    define_exception(ExceptionTTokenSetNotFoundToken, "Token nao encontrado")
    define_exception(ExceptionTTokenSetTokenSetting, "Falha em mudar o valor do token")
#endif

typedef int (*FTokenFinder)(char *token, void *data);

typedef struct TokenSet TTokenSet;

struct TokenSet {
	TList *tokens;
	TCount nofTokens;
};

/**
 *
 * Throws: ExceptionTTokenSetCreation
 * */
TTokenSet *TTokenSet_Create(TMemMgr *memmgr);
/**
 *
 * Throws: ExceptionTTokenSetCreation
 * */
TTokenSet *TTokenSet_CreateFromString(TMemMgr *memmgr, char *source, char *seps);
/**
 *
 * @return O token adicionado. Será, naturalmente, uma strng de endereço
 * diferente daquele do parâmetro 'token'
 *
 * Throws: ExceptionTTokenSetAddition
 * */
TString    TTokenSet_Add(TTokenSet *tokenset, char *token);

TString    TTokenSet_AddPreCreated(TTokenSet *tokenset, TString token);

TLstNod   *TTokenSet_Get(TTokenSet *tokenset, char *token);

void       TTokenSet_Del(TTokenSet *tokenset, TLstNod *node);
/**
 *
 * Throws: ExceptionToString
 * */
TString    TTokenSet_ToString(TTokenSet *tokenset, int charSep);


#endif /* MZ_TTOKENSET_H */
