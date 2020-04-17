
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

#include <string.h>
#define MZ_TokenSet_IMPLEMENT
#include "TokenSet.h"
#include "../base/TObject.h"
#include "../base/TList.h"
#include "../str/TString.h"

TList *TokenSet_CreateFromString(TMemMgr *memmgr, char *source, char *seps) {
	TList     *ret             = NULL;
	TString    sourceLocalCopy = NULL;
	TString    ztoken          = NULL;
	char      *ctoken;

	try_continue(ttknsetc_001)
		/* Criação do objeto */
		ret = TList_Create(memmgr); check(ttknsetc_001)

		/* Obtencao e adição de tokens */
		sourceLocalCopy = TString_Create(memmgr, source, 0); check(ttknsetc_001)

		ctoken = strtok(sourceLocalCopy, seps);
		if(!ctoken)
			throw_local_note(ttknsetc_001, Exception, "Nao ha tokens na string dada")

		while(ctoken) {
			ztoken = TString_Create(memmgr, ctoken, 0); check(ttknsetc_001)
			TokenSet_AddPreCreated(ret, ztoken);       check(ttknsetc_001)
			ctoken = strtok(NULL, seps);
			ztoken = NULL;
		}
	do_continue(ttknsetc_001)
		if(sourceLocalCopy)
			sourceLocalCopy = TObject_Destroy(sourceLocalCopy, NULL);
	end_continue(ttknsetc_001)
	catch_quickly(ttknsetc_001)
			if(ztoken) TObject_DestroyByPassDtor(ztoken, NULL);
			if(ret) TObject_Destroy(ret, NULL);
			throw_note(ExceptionTTokenSetCreation, NULL, _TRY_NOTE_)
	end_try(ttknsetc_001)

	return ret;
}

TString    TokenSet_Add(TList *tokenset, char *token) {
	TString ret=NULL;

	try(ttknsadd_001)
		ret = TString_Create(TObject_ManagerOf(tokenset), token, 0); check_note(ttknsadd_001, "Criacao de objeto para o token")
		TokenSet_AddPreCreated(tokenset, ret);                      check(ttknsadd_001)
	catch_quickly(ttknsadd_001)
		if(ret) TObject_Destroy(ret, NULL);
		throw_note(ExceptionTTokenSetAddition, NULL, _TRY_NOTE_)
	end_try(ttknsadd_001)

	return ret;
}

TString   TokenSet_ToString(TList *tokenset, int charSep) {
	TString ret;
	TSize   retEstimatedSize = tokenset->size * sizeof(char);
	char ssep[2] = { (char)charSep, 0 };

	LOOPLIST(tokenset,)
		retEstimatedSize += strlen((char *)_NODE_->item);
	END_LOOPLIST

	ret = TString_Create(TObject_ManagerOf(tokenset), NULL, retEstimatedSize);
	onerror(ret)
		throw_note(ExceptionToString, NULL, "TList conversion")

	LOOPLIST(tokenset,)
		strcat(ret, (char *)_NODE_->item);
		if(_NODE_->next) 
			strcat(ret, ssep);
	END_LOOPLIST

	return ret;
}

int token_tlistcallback(TNode *node, void *tkntext) {
	if(0==strcmp((char*)tkntext, (char *)node->item))
		return 1;
	return 0;
}

