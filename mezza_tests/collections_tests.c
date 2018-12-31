/*
 * collections_tests.c
 *
 *  Created on: 26 de dez de 2018
 *      Author: paioniu
 */
#include <mezza/SimpleTest.h>
#include <mezza/base/TMemMgr.h>
#include <mezza/base/TObject.h>
#include <mezza/collections/TTokenSet.h>
#include <mezza/collections/TMapEntry/TMapEntry.h>
#include <mezza/collections/TMapEntry/strvd.h>

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


TEST_IMPL_TEST(_mapentry_strvd_all) {
	TMapEntryStrVoid *entry1 = NULL;
	TMemMgr memmgr;
	TString key1;
	int  value1=1560;
	void *pvalue1 = &value1;
	
	key1 = TString_Create(TMemMgr_Init(&memmgr), "Exemplo 1 de chave", 0);
	entry1 = TMapEntryStrVoid_Create(&memmgr, key1, pvalue1);
	TEST_ASSERT_FATAL(entry1, "Falha de criação")
	TEST_ASSERT(entry1->key==key1, "Falha de inicializacao.")
	TEST_ASSERT(0==strcmp(entry1->key, key1), "Conteúdo da chave não corresponde ao fornecido.")
}

TEST_IMPL_TEST(collections_all) {
    TEST_CALL_TEST(_tokenset_all)
    TEST_CALL_TEST(_mapentry_strvd_all
     )
}

