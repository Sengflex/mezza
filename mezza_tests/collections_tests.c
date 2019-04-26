/*
 * collections_tests.c
 *
 *  Created on: 26 de dez de 2018
 *      Author: paioniu
 */
#include <mezza/SimpleTest.h>
#include <mezza/base/TMemMgr.h>
#include <mezza/base/TObject.h>
#include <mezza/collections/TMap.h>
#include <mezza/collections/TTokenSet.h>

TEST_MODULE

TEST_IMPL_TEST(_tokenset_all) {
	TTokenSet *tokenset1;
	TMemMgr memmgr;
	char *token1="Token 1";
	char *token2="Token 2";
	char *token3="Token 3";
	TString res;
	TString tostring1;
	TStatus statret;
	TLstNod *gottoken1 = NULL, *gottoken2 = NULL, *gottoken3 = NULL;
	char *source1 = "bola|da|vez|do|manuel";
	char *sep1 = "|";
	char *vector1[5] = {"bola", "da", "vez", "do", "manuel"};
	TPosition i = 0;

	tokenset1 = TTokenSet_Create(TMemMgr_Init(&memmgr));
	TEST_ASSERT_FATAL(tokenset1, "Falha de criacao de objeto")
	TEST_ASSERT(tokenset1->tokens, "Lista de tokens não foi criada")
	TEST_ASSERT(0==tokenset1->nofTokens, "'nofTokens' não foi inicializado")

	res = TTokenSet_Add(tokenset1, token1);
	TEST_ASSERT_FATAL(res, "Falha de retorno")
	TEST_ASSERT(res!=token1, "Falha de retorno. Retorno deveria ser um endereço diferente da origem")
	TEST_ASSERT(0==strcmp(res, token1), "Falha de retorno. Retorno deveria conter o mesmo valor/texto da origem")
	TEST_ASSERT(1==tokenset1->nofTokens, "'nofTokens' de tokens nao atualizado")

	res = TTokenSet_Add(tokenset1, token2);
	TEST_CHECK_PREPARATION(res)
	res = TTokenSet_Add(tokenset1, token3);
	TEST_CHECK_PREPARATION(res)

	gottoken2 = TTokenSet_Get(tokenset1, token2);
	TEST_ASSERT_FATAL(gottoken2, "Falha de retorno")
	TEST_ASSERT((char*)gottoken2->item!=token2, "Falha de retorno. Retorno deveria ser um endereço diferente da origem")
	TEST_ASSERT(0==strcmp((char*)gottoken2->item, token2), "Falha de retorno. Retorno deveria conter o mesmo valor/texto da origem")


	gottoken3 = TTokenSet_Get(tokenset1, token3);
	TEST_CHECK_PREPARATION(gottoken2)
	TTokenSet_Del(tokenset1, gottoken3);
	gottoken3 = TTokenSet_Get(tokenset1, token3);
	TEST_ASSERT(NULL==gottoken3, "Falha de retorno. Token nao mais deveria ser encontrado")

	tostring1 = TTokenSet_ToString(tokenset1, '=');
	TEST_ASSERT_FATAL(tostring1, "Falha de retorno")
	TEST_ASSERT(0==strcmp(tostring1, "Token 1=Token 2"), "Falha de retorno. String errada")

	tokenset1 = TObject_Destroy(tokenset1, NULL);
	tokenset1 = TTokenSet_CreateFromString(&memmgr, source1, sep1);
	TEST_ASSERT_FATAL(tokenset1, "Falha de retorno")

	LOOPLIST(tokenset1->tokens,)
	TEST_ASSERT(0==strcmp((char*)_NODE_->item, vector1[i]), "Tokens gerados estao errados")
	i++;
	END_LOOPLIST

	tokenset1 = TObject_Destroy(tokenset1, NULL);
}

TEST_IMPL_TEST(_mapentry_all) {
	TMapEntry *entry1 = NULL;
	TMapEntry *entry2 = NULL;
	TMapEntry *entry3 = NULL;
	TMemMgr memmgr;
	TString key1;
	TString key2;
	TString key3;
	int  value1=1560;
	void *pvalue1 = &value1;
	TString value2;
	TString value3;
	
	key1 = TString_Create(TMemMgr_Init(&memmgr), "Exemplo 1 de chave", 0);
	entry1 = TMapEntry_Create(&memmgr, key1, pvalue1);
	TEST_ASSERT_FATAL(entry1, "Falha de criação")
	TEST_ASSERT(entry1->key==key1, "Falha de inicializacao.")
	TEST_ASSERT(0==strcmp(entry1->key, key1), "Conteúdo da chave não corresponde ao fornecido.")
	TEST_ASSERT(entry1->value == pvalue1, "Valor da chave está errado")

	key2 = TString_Create(&memmgr, "Exemplo 2 de chave", 0);
	TEST_CHECK_PREPARATION(key2)
	value2 = TString_Create(&memmgr, "Exemplo 2 de valor", 0);
	TEST_CHECK_PREPARATION(value2)
	TEST_CHECK_PREPARATION(TRUE == TObject_Exists(&memmgr, value2))

	entry2 = TMapEntry_CreateObj(&memmgr, key2, value2);
	TEST_CHECK_PREPARATION(entry2)
	TObject_Destroy(entry2, NULL);
	TEST_ASSERT(FALSE == TObject_Exists(&memmgr, value2), "Valor 2 não deveria existir mais, pois é um TObject")


	key3 = TString_Create(&memmgr, "Exemplo 3 de chave", 0);
	TEST_CHECK_PREPARATION(key3)
	value3 = TString_Create(&memmgr, "Exemplo 3 de valor", 0);
	TEST_CHECK_PREPARATION(value3)
	entry3 = TMapEntry_Create(&memmgr, key3, value3);
	TEST_CHECK_PREPARATION(entry3)
	TObject_Destroy(entry3, NULL);
	TEST_ASSERT(TRUE == TObject_Exists(&memmgr, value3), "Valor 3 deveria existir ainda, embora seja um TObject")

	TObject_Destroy(entry1, NULL);
	TEST_ASSERT(FALSE == TObject_Exists(&memmgr, key1), "Chave 1 não deveria existir mais, pois é um TObject")

}

TEST_IMPL_TEST(_map_all) {
	TMap *map1;
	TMemMgr memmgr;
	TString key1;
	int value1=191;
	TString key2;
	int value2=192;
	TString key3;
	int value3=193;
	void *pvalue1=&value1;
	void *pvalue2=&value2;
	void *pvalue3=&value3;
	void *ret1;
	void *retValueForKey2;
	TString key4;
	TString key5;
	TString value4;

	map1 = TMap_Create(TMemMgr_Init(&memmgr));
	TEST_ASSERT(map1, "Falha de criação")

	TEST_CHECK_PREPARATION(key1 = TString_Create(&memmgr, "key one", 0))
	ret1 = TMap_SetEntry(map1, key1, pvalue1);
	TEST_ASSERT(ret1==pvalue1, "Falha de retorno")

	TEST_CHECK_PREPARATION(key2 = TString_Create(&memmgr, "key two", 0))
	TEST_CHECK_PREPARATION(TMap_SetEntry(map1, key2, pvalue2))
	TEST_CHECK_PREPARATION(key3 = TString_Create(&memmgr, "key three", 0))
	TEST_CHECK_PREPARATION(TMap_SetEntry(map1, key3, pvalue3))

	retValueForKey2 = TMap_GetEntry(map1, "key two");
	TEST_ASSERT(retValueForKey2==pvalue2, "Falha de obtenção de valor da chave")

	retValueForKey2 = TMap_SetEntry(map1, "key two", pvalue3);
	TEST_ASSERT(retValueForKey2==pvalue3, "Falha de retorno")

	retValueForKey2 = TMap_GetEntry(map1, "key two");
	TEST_ASSERT(retValueForKey2==pvalue3, "Falha de obtenção de valor da chave após reatribuição")

	TMap_UnsetEntry(map1, "key two");
	retValueForKey2 = TMap_GetEntry(map1, "key two");
	TEST_ASSERT(retValueForKey2==NULL, "Falha de retorno. Chave não deveria existir")
	
	TMap_UnsetEntry(map1, "key one");
	TEST_ASSERT(FALSE == TObject_Exists(&memmgr, key1), "Key1 não deveria mais existir")

	key4 = TString_Create(&memmgr, "key four", NULL);
	TEST_CHECK_PREPARATION(key4)
	value4 = TString_Create(&memmgr, "value four", NULL);
	TEST_CHECK_PREPARATION(value4)
	TMap_SetEntry(map1, key4, value4);
	TMap_UnsetEntry(map1, key4);
	TEST_ASSERT(TRUE == TObject_Exists(&memmgr, value4), "Value4 ainda deveria existir")

	key5 = TString_Create(&memmgr, "key five", NULL);
	TEST_CHECK_PREPARATION(key5)
	TMap_SetEntryObj(map1, key5, value4);
	TMap_UnsetEntry(map1, "key five");
	TEST_ASSERT(FALSE == TObject_Exists(&memmgr, value4), "Value4 não deveria mais existir")
}

void dtor1(TObject obj, void *extra) {
	int *count = (int*)extra;

	(*count)++;
}

TEST_IMPL_TEST(_map_val_as_obj) {
	TMemMgr memmgr;
	TString val1, key1;
	TMap *map1;
	int dcount = 0;

	val1 = TString_Create(TMemMgr_Init(&memmgr), "str 1", 0);
	TEST_CHECK_PREPARATION(val1)
	TObject_SetDtor(val1, dtor1);
	map1 = TMap_Create(&memmgr);
	TEST_CHECK_PREPARATION(map1)
	key1 = TString_Create(&memmgr, "unico", 0);
	TEST_CHECK_PREPARATION(key1)
	TEST_CHECK_PREPARATION(TMap_SetEntryObj(map1, key1, val1))
	TMap_UnsetEntryWithUserData(map1, key1, &dcount);
	TEST_ASSERT(dcount == 1, "dcount deveria ser igual a 1")
}

TEST_IMPL_TEST(collections_all) {
    TEST_CALL_TEST(_tokenset_all)
    TEST_CALL_TEST(_mapentry_all)
    TEST_CALL_TEST(_map_all)
    TEST_CALL_TEST(_map_val_as_obj)
}

