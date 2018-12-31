/*
 * str_tests.c
 *
 *  Created on: 11 de dez de 2018
 *      Author: root
 */


#include <mezza/SimpleTest.h>
#include <mezza/OnError.h>
#include <mezza/Lang.h>
#include <mezza/base/TObject.h>
#include <mezza/str/TString.h>
#include <mezza/str/StringUtils.h>
#include <mezza/str/StringBuilder.h>
#include <mezza/str/TStringVector.h>

TEST_MODULE

TEST_IMPL_TEST(_str_create) {
    TString str1;
    TMemMgr memmgr;
    char *strsrc1 = "Fonte de texto";
    TSize strsrc1_len = strlen(strsrc1);

    str1 = TString_Create(TMemMgr_Init(&memmgr), NULL, 0);
    TEST_ASSERT_FATAL(str1, "Falha de criação")

    TEST_ASSERT(TString_SizeOf(str1) == 0, "Falha de criação. Tamanho incorreto")

    TEST_ASSERT(*str1 == 0, "Falha de criação. Conteúdo errado")

    str1 = TObject_Destroy(str1, NULL);

    str1 = TString_Create((&memmgr), strsrc1, 0);
    TEST_ASSERT_FATAL(str1, "Falha de criação")

    TEST_ASSERT(0 == strcmp(strsrc1, str1), "Falha de criação. Conteúdo errado")
    TEST_ASSERT(TString_SizeOf(str1) == strsrc1_len, "Falha de metadados. Tamanhoerrado")

    str1 = TObject_Destroy(str1, NULL);

    str1 = TString_Create((&memmgr), strsrc1, strsrc1_len + 100);
    TEST_ASSERT_FATAL(str1, "Falha de criação")

    TEST_ASSERT(0 == strcmp(strsrc1, str1), "Falha de criação. Conteúdo errado")
    TEST_ASSERT(TString_SizeOf(str1) == strsrc1_len + 100, "Falha de metadados. Tamanhoerrado")

    str1 = TObject_Destroy(str1, NULL);

    str1 = TString_Create((&memmgr), NULL, strsrc1_len + 100);
    TEST_ASSERT_FATAL(str1, "Falha de criação")

    TEST_ASSERT(0 == *str1, "Falha de criação. Conteúdo errado")
    TEST_ASSERT(TString_SizeOf(str1) == strsrc1_len + 100, "Falha de metadados. Tamanhoerrado")

    str1 = TObject_Destroy(str1, NULL);

    str1 = TString_Create((&memmgr), strsrc1, 1);
    TEST_ASSERT_FATAL(str1, "Falha de criação")

    TEST_ASSERT(TString_SizeOf(str1) == strsrc1_len, "Falha de metadados. Tamanhoerrado")

    str1 = TObject_Destroy(str1, NULL);

}

TEST_IMPL_TEST(_str_copy) {
    TString str1;
    char *srcstr1="String de teste";
    char *srcstr2="String";
    TSize srcstr1len = strlen(srcstr1);
    TSize srcstr2len = strlen(srcstr2);
    TMemMgr memmgr;
    TString copyRes1;

    str1 = TString_Create(TMemMgr_Init(&memmgr), NULL, srcstr1len);
    TEST_CHECK_PREPARATION(str1)
    copyRes1 = TString_Copy(&str1, srcstr1, srcstr1len);
    TEST_ASSERT(copyRes1, "Falha de retorno")

    TEST_ASSERT(0 == strcmp(str1, srcstr1), "Falha de cópia")

    TEST_ASSERT(TString_SizeOf(str1) == srcstr1len, "Falha de metadados. Tamanhoerrado")

    TString_Copy(&str1, srcstr1, srcstr2len);

    TEST_ASSERT(0 == strcmp(str1, srcstr2), "Falha de cópia")

    str1 = TObject_Destroy(str1, NULL);
    str1 = TString_Create(&memmgr, NULL, srcstr2len);
    TString_Copy(&str1, srcstr1, 0);

    TEST_ASSERT(0 == strcmp(str1, srcstr1), "Falha de cópia")

    TEST_ASSERT(TString_SizeOf(str1) == srcstr1len, "Falha de metadados. Tamanhoerrado")

    {
        char *format1 = "%s %d";
        int number1 = 109;

        TString_CopyF(&str1, format1, srcstr1, number1);

        TEST_ASSERT(0 == strcmp(str1, "String de teste 109"), "Falha de cópia formatada")

        TEST_ASSERT(TString_SizeOf(str1) == srcstr1len+16, "Falha de metadados. Tamanhoerrado")

    }
}

TEST_IMPL_TEST(_str_cat__cmp) {
    TString str1;
    TString tmp;
    TMemMgr memmgr;
    char *orig1 = " mas";

    str1 = TString_Create(TMemMgr_Init(&memmgr), "das", 0);
    TEST_CHECK_PREPARATION(str1)

    tmp = TString_Concat(&str1, orig1);

    TEST_ASSERT_FATAL(tmp, "Falha de concat")
    str1 = tmp;

    TEST_ASSERT(0 == strcmp(str1, "das mas"), "Falha de concat. COnteudo")

    TEST_ASSERT(TString_SizeOf(str1) == 7, "Falha de metadados. Tamanho errado")


    tmp = TString_ConcatF(&str1, " %s %d", "olou", 123);

    TEST_ASSERT_FATAL(tmp, "Falha de concat")
    str1 = tmp;

    TEST_ASSERT(0 == strcmp(str1, "das mas olou 123"), "Falha de concat. COnteudo")

    TEST_ASSERT(TString_SizeOf(str1) == 28, "Falha de metadados. Tamanho errado")

    TEST_ASSERT(0 == StringUtils_CompareF(str1, "das mas %s %d", "olou", 123), "Falha de comparação formatada")

    TString_Resize(&str1, 40);

    TEST_ASSERT(TString_SizeOf(str1) == 40, "Falha de metadados. Tamanho errado")

    TString_Resize(&str1, 16);

    TEST_ASSERT(TString_SizeOf(str1) == 16, "Falha de metadados. Tamanho errado")

    TEST_ASSERT(0 == strcmp(str1, "das mas olou 123"), "Falha de concat. COnteudo")

    TString_Resize(&str1, 7);

    TEST_ASSERT(TString_SizeOf(str1) == 7, "Falha de metadados. Tamanho errado")

    TEST_ASSERT(0 == strcmp(str1, "das mas"), "Falha de concat. COnteudo")

}

TEST_IMPL_TEST(_sscanf) {
    char *srcStr = "Paulo Gomes, 35 anos. Locutor de rádio";
    char *format = "%, % anos. %";
    TString nome = NULL;
    TString idade = NULL;
    TString profissao = NULL;
    TString res;
    TMemMgr memmgr;

    res = StringBuilder_FromString(TMemMgr_Init(&memmgr), srcStr, format, &nome, &idade, &profissao);
    TEST_ASSERT(res, "Falha de retorno")
    TEST_ASSERT(res == nome, "Falha de retorno. Primeira string gerada não retornada")

    TEST_ASSERT_FATAL(nome, "Falha de retorno")
    TEST_ASSERT(0 == strcmp("Paulo Gomes", nome), "Falha de geração. Conteúdo")

    TEST_ASSERT(TString_SizeOf(nome) == 11, "Falha de metadados. Tamanho errado")

    TEST_ASSERT_FATAL(idade, "Falha de retorno")
    TEST_ASSERT(0 == strcmp("35", idade), "Falha de geração. Conteúdo")

    TEST_ASSERT(TString_SizeOf(idade) == 2, "Falha de metadados. Tamanho errado")

    TEST_ASSERT_FATAL(profissao, "Falha de retorno")
    TEST_ASSERT(0 == strcmp("Locutor de rádio", profissao), "Falha de geração. Conteúdo")

    TEST_ASSERT(TString_SizeOf(profissao) == strlen("Locutor de rádio"), "Falha de metadados. Tamanho errado")

}

TEST_IMPL_TEST(str_file) {
    char *fileContent1 = "domarg@spoilt.byli";
    char *fileContent2 = "domarg@spoilt";
    char *filename = "mezza.str.001.tests";
    FILE *fp = fopen(filename, "w+");
    TMemMgr memmgr;
    TString str1 = NULL;
    TString res;

    fwrite(fileContent1, sizeof(char), strlen(fileContent1), fp);
    fclose(fp);
    fp = fopen(filename, "r");

    res = StringBuilder_FromFile(TMemMgr_Init(&memmgr), &str1, fp);
    fclose(fp);
    remove(filename);

    TEST_ASSERT(res, "Falha de retorno")
    TEST_ASSERT_FATAL(str1, "Falha de geração. COnteúdo")
    TEST_ASSERT(res==str1, "Falha de retorno")
    TEST_ASSERT(0 == strcmp(fileContent1, str1), "Falha de geração. COnteúdo")
    TEST_ASSERT(strlen(fileContent1) == TString_SizeOf(str1), "Falha de metadados. Tamanho")

    fp = fopen(filename, "w+");
    fwrite(fileContent1, sizeof(char), strlen(fileContent1), fp);
    fclose(fp);
    fp = fopen(filename, "r");
    str1 = TObject_Destroy(str1, NULL);
    res = StringBuilder_FromFileSection(&memmgr, &str1, fp, '.');

    TEST_ASSERT(res, "Falha de retorno")
    TEST_ASSERT_FATAL(str1, "Falha de geração. COnteúdo")
    TEST_ASSERT(res==str1, "Falha de retorno")
    TEST_ASSERT(0 == strcmp(fileContent2, str1), "Falha de geração. COnteúdo")
    TEST_ASSERT(strlen(fileContent2) == TString_SizeOf(str1), "Falha de metadados. Tamanho")

    fclose(fp);
    remove(filename);
}

TEST_IMPL_TEST(str_trunc) {
    char *str1 = "solmodo.guhauls.com";
    char *trunc1;

    trunc1 = StringUtils_TruncRev(str1, '.');
    TEST_ASSERT_FATAL(trunc1, "Falha de retorno")

    TEST_ASSERT(0 == strcmp(trunc1, "com"), "Conteúdo errado")

    trunc1 = StringUtils_TruncRevSkip(str1, '.', 1);
    TEST_ASSERT_FATAL(trunc1, "Falha de retorno")

    TEST_ASSERT(0 == strcmp(trunc1, "guhauls.com"), "Conteúdo errado")

}

TEST_IMPL_TEST(str_calcszfmt) {
    char *format1 = "%s, %d, %s, %f, %f";
    char *_20str  = "O louco morreu velho";
    int   _96age  = 96;
    char *_30str  = "quase louco era ele ao comecar";
    float taxa    = 5.6f;
    double taxa2  = 19000005.6088874;
    TSize size1;
    TSize expectedSize1 = 143;

    size1 = StringUtils_CalcSizeF(format1, _20str, _96age, _30str, taxa, taxa2);
    TEST_ASSERT(size1 == expectedSize1, "Tamanho errado")
}

TEST_IMPL_TEST(str_trim) {
    TString str1;
    TString str2;
    TMemMgr memmgr;

    str1 = TString_Create(TMemMgr_Init(&memmgr), " string with spaces  ", 0);
    str2 = TString_Create(&memmgr, "*% $ string with symbols*$ %  ", 0);

    StringUtils_Trimm(str1, ' ');
    TEST_ASSERT(0 == strcmp(str1, "string with spaces"), "Falha de trim")

    StringUtils_TrimmAll(str2, " *$%");
    TEST_ASSERT(0 == strcmp(str2, "string with symbols"), "Falha de trim")
}

TEST_IMPL_TEST(str_vstr) {
    TString *vstr1;
    char *content1[5] = {
            "Olá cachorro",
            "Cai folha",
            "Pode passar",
            "Ate aqui so",
            "Talvez ele va"
    };
    TMemMgr memmgr;

    vstr1 = TStringVector_Create(TMemMgr_Init(&memmgr), content1, 5);
    TEST_ASSERT_FATAL(vstr1, "Alocação de vetor")

    loop_i(5)
        TEST_ASSERT(0 == strcmp(vstr1[_INDEX_], content1[_INDEX_]), "Falha de atribuição")
    end_loop_i

    TStringVector_Reset(vstr1, 5);

    loop_i(5)
        TEST_ASSERT(vstr1[_INDEX_] == NULL, "Falha de reset")
    end_loop_i
}

TEST_IMPL_TEST(str_all) {
    TEST_CALL_TEST(_str_create)
    TEST_CALL_TEST(_str_copy)
    TEST_CALL_TEST(_str_cat__cmp)
    TEST_CALL_TEST(_sscanf)
    TEST_CALL_TEST(str_file)
    TEST_CALL_TEST(str_trunc)
    TEST_CALL_TEST(str_calcszfmt)
    TEST_CALL_TEST(str_trim)
    TEST_CALL_TEST(str_vstr)
}



