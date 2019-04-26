#define MZ_CONF_IMPLEMENT
#include "Conf.h"

#include <stdio.h>
#include <string.h>

#include "FileUtil.h"
#include "base/TMemMgr.h"
#include "base/TObject.h"
#include "base/TList.h"
#include "str/StringBuilder.h"
#include "str/TString.h"
#include "collections/TMap.h"
#include "collections/TTokenSet.h"

TStatus Conf_LoadFromFile(TMap *confsMap, char *filename) {
	FILE      *fileStream  = NULL;
	TString    fileContent = NULL,
			   token       = NULL,
			   key         = NULL,
			   val         = NULL;
	TList     *tokenSet    = NULL;
	TMemMgr   *memmgr = TObject_ManagerOf(confsMap);

	try_continue(tcscff_001)
		fileStream = fopen(filename, "r+");
		if(!fileStream)
			throw_note(ExceptionConfLoad, FAIL, exception_msg(ExceptionFIleOpening))

		StringBuilder_FromFile(memmgr, &fileContent, fileStream); check_note(tcscff_001, "Leitura do arquivo")

		tokenSet = TokenSet_CreateFromString(memmgr, fileContent, "\n"); check_note(tcscff_001, "Tokenização")

		LOOPLIST(tokenSet,)
			token = (TString)_NODE_->item;
			StringBuilder_FromString(memmgr, token, "%=%", &key, &val); check_note(tcscff_001, "Extração de par")
			TMap_SetEntryObj(confsMap, key, val); check_note(tcscff_001, "Inserção de par")
			key = NULL;
			val = NULL;
		END_LOOPLIST

	do_continue(tcscff_001)
		if(tokenSet) TObject_Destroy(tokenSet, NULL);

		if(fileContent) TObject_DestroyByPassDtor(fileContent, NULL);

		if(fileStream) fclose(fileStream);
	end_continue(tcscff_001)

	catch(tcscff_001)
		on(Exception)
			if(confsMap) {
				if(key) TObject_Destroy(key, NULL);
				if(val) TObject_Destroy(val, NULL);
				TObject_Destroy(confsMap, NULL);
			}
			throw_note(ExceptionConfLoad, FAIL, _TRY_NOTE_)
		end_on
	end_try(tcscff_001)

	return OK;
}

TStatus   Conf_SaveToFile(TMap *confsMap, char *filename) {
	FILE     *fileStream  = NULL;
	TString tempFilename;
	int res;
	
	tempFilename = TString_Create(TObject_ManagerOf(confsMap), NULL, strlen(filename) + 5);
	if(!tempFilename)
		throw_note(ExceptionConfSave, FAIL, "Gerando nome de arquivo temporário")
	
	sprintf(tempFilename, "~%s", filename);

	fileStream = fopen(tempFilename, "w");
	if(!fileStream)
		throw_note(ExceptionConfSave, FAIL, "Abrindo arquivo temporario para gravação")

	LOOPLIST(confsMap->entries, )
		res = fprintf( fileStream,
				 "%s=%s\n",
				 ((TMapEntry *)_NODE_->item)->key,
				 (char*)((TMapEntry *)_NODE_->item)->value);
		onerror(res > -1) {
			fclose(fileStream);
			remove(tempFilename);
			throw_note(ExceptionConfSave, FAIL, "Gravando entrada no arquivo")
		}
	END_LOOPLIST

	fclose(fileStream);
	res = rename(tempFilename, filename);
	onerror(res==0) {
		remove(tempFilename);
		throw_note(ExceptionConfSave, FAIL, "Renomeando arquivo temporaio")
	}

	return OK;
}

TStatus Conf_Set(TMap *confsMap, char *conf, char *value) {
	TLstNod          *entryNode = NULL;
	TMapEntry *entry     = NULL;
	TString           key       = NULL;
	TString           value2    = NULL;

	try(cnfst_001)
		entryNode = TMap_GetEntryNode(confsMap, conf);

		if(entryNode) {
			entry = (TMapEntry *)entryNode->item;
			TString_Copy(entry->value, value, 0); 
				check_note(cnfst_001, "Copiando novo valor de configuração")
		} else {

			key = TString_Create(TObject_ManagerOf(confsMap), conf, 0);
				check_note(cnfst_001, "Alocando memória para a chave")

			value2 = TString_Create(TObject_ManagerOf(confsMap), value, 0);
				check_note(cnfst_001, "Alocando memória para o valor")

			TMap_SetEntryObj(confsMap, key, value2);
				check_note(cnfst_001, "Adicinando novo par ao mapa")
		}
	catch_quickly(cnfst_001)
		if(key) TObject_Destroy(key, NULL);
		if(value2) TObject_Destroy(value2, NULL);
		throw_note(ExceptionConfSet, FAIL, _TRY_NOTE_)
	end_try(cnfst_001)

	return OK;
}

TString Conf_Get(TMap *confsMap, char *conf) {
	TLstNod          *entryNode = NULL;

	entryNode = TMap_GetEntryNode(confsMap, conf);

	onerror(entryNode)
		throw(ExceptionConfNotFound, NULL)

	return ((TMapEntry *)entryNode->item)->value;
}
