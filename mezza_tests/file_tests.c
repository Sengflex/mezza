/*
 * file_tests.c
 *
 *  Created on: 12 de dez de 2018
 *      Author: root
 */

#include <mezza/SimpleTest.h>
#include <mezza/base/TMemMgr.h>
#include <mezza/FileUtil.h>

TEST_MODULE

TEST_IMPL_TEST(_file_all) {
    char *filename = "mezza.file.001.tests";
    FILE *fp = fopen(filename, "w");
    char *content = "conteudo para por no arquivo@conteudo adicional-12000";
    TSize contentSz = strlen(content);
    TSize fsize, count;

    fwrite(content, sizeof(char), contentSz, fp);
    fclose(fp);
    fp = fopen(filename, "r");

    fsize = FileUtil_SizeOf(fp);
    TEST_ASSERT(fsize==contentSz, "Tamanho lido está errado")

    fseek(fp, 9, SEEK_SET);
    fsize = FileUtil_TailSizeOf(fp);
    TEST_ASSERT(fsize==contentSz-9, "Tamanho lido está errado")

    fsize = FileUtil_SectionSizeOf(fp, '@');
    TEST_ASSERT(fsize==19, "Tamanho lido está errado")

    fsize = FileUtil_PositionOf(fp, '@');
    TEST_ASSERT(fsize==28, "Tamanho lido está errado")

    fseek(fp, 0, SEEK_SET);
    count = FileUtil_SkipChars(fp, "conteudo ");
    TEST_ASSERT(count==9, "Tamanho lido está errado")
    TEST_ASSERT(ftell(fp)==9, "Posição está errado")

    count = FileUtil_PositionAtChar(fp, '@');
    TEST_ASSERT(count==28, "Tamanho lido está errado")
    TEST_ASSERT(ftell(fp)==28, "Posição está errado")

    fclose(fp);
    remove(filename);
}

TEST_IMPL_TEST(file_all) {
    TEST_CALL_TEST(_file_all)
}

