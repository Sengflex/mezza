/*
 * file_tests.c
 *
 *  Created on: 12 de dez de 2018
 *      Author: root
 */

#include <mezza/Type.h>
#include <mezza/SimpleTest.h>
#include <mezza/base/TMemMgr.h>
#include <mezza/base/TObject.h>
#include <mezza/Conf.h>
#include <mezza/collections/Map.h>

TEST_MODULE

TEST_IMPL_TEST(_conf_all) {
    TList *map1;
    TList *map2;
    TMemMgr memmgr;
    char *filename1 = "conf_tests_001.txt";
    FILE *filestream1;
    TStatus ret;
    TString key1;
    TString value1;
    TString key2;
    TString value2;
    TString key3;
    TString value3;
    TString retvalue1;
    TMapEntry *entry1;
    char *keys[3] = {"conf1", "conf2", "conf3"};
    char *values[3] = {"170", "author of", "date of"};

    TEST_CHECK_PREPARATION(map1 = TList_Create(TMemMgr_Init(&memmgr)))
    TEST_CHECK_PREPARATION(map2 = TList_Create(&memmgr))

    ret = Conf_Set(map1, keys[0], values[0]);
    TEST_ASSERT(ret==OK, "Falha de retorno")
    ret = Conf_Set(map1, keys[1], values[1]);
    TEST_CHECK_PREPARATION(ret==OK)
    ret = Conf_Set(map1, keys[2], values[2]);
    TEST_CHECK_PREPARATION(ret==OK)

    retvalue1 = Conf_Get(map1, keys[1]);
    TEST_ASSERT(retvalue1, "Retorno de erro")
    TEST_ASSERT(0==strcmp(retvalue1, values[1]), "Conteudo ");

    ret = Conf_SaveToFile(map1, filename1);
    TEST_ASSERT(ret==OK, "Retorno de erro")

    ret = Conf_LoadFromFile(map2, filename1);
    TEST_ASSERT(ret==OK, "Retorno de erro")
    TEST_ASSERT(map2->size==3, "Número de entradas inválido")

    retvalue1 = Conf_Get(map2, keys[0]);
    TEST_ASSERT(retvalue1, "Retorno de erro")
    TEST_ASSERT(0==strcmp(retvalue1, values[0]), "Conteudo ");

    retvalue1 = Conf_Get(map2, keys[1]);
    TEST_ASSERT(retvalue1, "Retorno de erro")
    TEST_ASSERT(0==strcmp(retvalue1, values[1]), "Conteudo ");

    retvalue1 = Conf_Get(map2, keys[2]);
    TEST_ASSERT(retvalue1, "Retorno de erro")
    TEST_ASSERT(0==strcmp(retvalue1, values[2]), "Conteudo ");

    TObject_Destroy(map1, NULL);
    remove(filename1);
    TMemMgr_FreeAll(&memmgr);
}

TEST_IMPL_TEST(conf_all) {
    TEST_CALL_TEST(_conf_all)
}

