/*
 * TTokenSet.c
 *
 *  Created on: 21 de dez de 2018
 *      Author: root
 */

#include <string.h>
#define MZ_TTOKENSET_IMPLEMENT
#include "TTokenSet.h"
#include "../base/TObject.h"
#include "../base/TList.h"

static void tokenSetCleaner(TObject obj, void *extra);

TTokenSet *TTokenSet_Create(TMemMgr *memmgr) {
	TTokenSet *ret             = NULL;

	ret = TObject_Create(memmgr, sizeof(TTokenSet), tokenSetCleaner);
	if(!ret)
		throw(ExceptionTTokenSetCreation, NULL)
	ret->tokens = TList_Create(memmgr);
	if(!ret->tokens) {
		TObject_DestroyByPassDtor(ret, NULL);
		throw(ExceptionTTokenSetCreation, NULL)
	}
	ret->nofTokens = 0;

	return ret;
}

TTokenSet *TTokenSet_CreateFromString(TMemMgr *memmgr, char *source, char *seps) {
	TTokenSet *ret_tokenset             = NULL;
	TString    sourceLocalCopy = NULL;
	TString    ztoken          = NULL;
	char      *ctoken;

	try_continue(ttknsetc_001)
		/* Criação do objeto */
		ret_tokenset = TTokenSet_Create(memmgr); check(ttknsetc_001)

		/* Obtencao e adição de tokens */
		sourceLocalCopy = TString_Create(memmgr, source, 0); check(ttknsetc_001)

		ctoken = strtok(sourceLocalCopy, seps);
		if(!ctoken)
			throw_local_note(ttknsetc_001, Exception, "Nao ha tokens na string dada")

		while(ctoken) {
			ztoken = TString_Create(memmgr, ctoken, 0); check(ttknsetc_001)
			TTokenSet_AddPreCreated(ret_tokenset, ztoken);       check(ttknsetc_001)
			ctoken = strtok(NULL, seps);
			ztoken = NULL;
		}
	do_continue(ttknsetc_001)
		if(sourceLocalCopy)
			sourceLocalCopy = TObject_Destroy(sourceLocalCopy, NULL);
	end_continue(ttknsetc_001)
	catch_quickly(ttknsetc_001)
			if(ztoken) TObject_DestroyByPassDtor(ztoken, NULL);
			if(ret_tokenset) TObject_Destroy(ret_tokenset, NULL);
			throw_note(ExceptionTTokenSetCreation, NULL, _TRY_NOTE_)
	end_try(ttknsetc_001)

	return ret_tokenset;
}

TString    TTokenSet_Add(TTokenSet *tokenset, char *token) {
	TString ret=NULL;

	try(ttknsadd_001)
		ret = TString_Create(TObject_ManagerOf(tokenset), token, 0); check_note(ttknsadd_001, "Criacao de objeto para o token")
		TTokenSet_AddPreCreated(tokenset, ret);                      check(ttknsadd_001)
	catch_quickly(ttknsadd_001)
		if(ret) TObject_Destroy(ret, NULL);
		throw_note(ExceptionTTokenSetAddition, NULL, _TRY_NOTE_)
	end_try(ttknsadd_001)

	return ret;
}
TString    TTokenSet_AddPreCreated(TTokenSet *tokenset, TString token) {
	onerror(TList_Add(tokenset->tokens, token))
		throw_note(ExceptionTTokenSetAddition, NULL, "Adicao 'a lista")

	tokenset->nofTokens++;

	return token;
}

TString   TTokenSet_ToString(TTokenSet *tokenset, int charSep) {
	TString ret;
	TSize   retEstimatedSize = tokenset->nofTokens * sizeof(char);

	LOOPLIST(tokenset->tokens,)
		retEstimatedSize += strlen((char *)_NODE_->item);
	END_LOOPLIST

	ret = TString_Create(TObject_ManagerOf(tokenset), NULL, retEstimatedSize);
	onerror(ret)
		throw_note(ExceptionToString, NULL, "TTokenSet conversion")

	LOOPLIST(tokenset->tokens,)
		strcat(ret, (char *)_NODE_->item);
		if(_NODE_->next) {
			char ssep[2] = { (char)charSep, 0 };
			strcat(ret, ssep);
		}
	END_LOOPLIST

	return ret;
}

TLstNod   *TTokenSet_Get(TTokenSet *tokenset, char *token) {
	LOOPLIST(tokenset->tokens,)
		if(0==strcmp(token, (char *)_NODE_->item))
			return _NODE_;
	END_LOOPLIST

	return NULL;
}

void       TTokenSet_Del(TTokenSet *tokenset, TLstNod *node) {
	TList_RemTObject(tokenset->tokens, node);
}

static void tokenSetCleaner(TObject obj, void *extra) {
	TTokenSet *ret = (TTokenSet *)obj;

	if(ret->tokens) {
		if(ret->tokens->start)
			TList_ForeachDoDestroy(ret->tokens, NULL);
		TObject_Destroy(ret->tokens, NULL);
	}
}
