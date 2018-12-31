/*
 * object_tests.c
 *
 *  Created on: 10 de dez de 2018
 *      Author: root
 */
#include <mezza/SimpleTest.h>
#include <mezza/base/TObject.h>
#include <mezza/OnError.h>

TEST_MODULE

static void dtor1(TObject obj, void *data) {
    TSize *size = data;

    *size = TObject_SizeOf(obj);
}

TEST_IMPL_TEST(_object_all) {
    TObject obj1;
    TObject obj2;
    TMemMgr memmgr;
    TMemMgr *memmgr2;
    TSize sizeInBytes = 100;
    TSize size1;
    TBool res;
    TSize szDtor=0;

    TMemMgr_Init(&memmgr);

    obj1 = TObject_Create(&memmgr, sizeInBytes, NULL);
    TEST_ASSERT(obj1, "Alocação de objetos")

    res = TObject_Exists(&memmgr, obj1);
    TEST_ASSERT(res ==  TRUE, "Detecção de existência")

    size1 = TObject_SizeOf(obj1);
    TEST_ASSERT(size1 ==  sizeInBytes, "Obtenção de tamanho")

    memmgr2 = TObject_ManagerOf(obj1);
    TEST_ASSERT(memmgr2 ==  &memmgr, "Obtenção de manager")

    obj2 = TObject_Resize(obj1, sizeInBytes * 2);
    TEST_ASSERT_FATAL(obj2, "Falha de retorno de objeto")
    obj1 = obj2;

    TEST_ASSERT(TObject_SizeOf(obj2) ==  sizeInBytes * 2, "Atualização de metadados. Tamanaho")

    TObject_Destroy(obj1, NULL);
    res = TObject_Exists(&memmgr, obj1);
    TEST_ASSERT(res ==  FALSE, "Detecção de inexistência")

    obj1 = TObject_Create(&memmgr, sizeInBytes, NULL);
    onerror(obj1) {
        SHOWERROR("mezza_tests.object_tests._object_all",
                "Falha de criação de objeto")
        return;
    }
    TObject_SetDtor(obj1, dtor1);
    TObject_Destroy(obj1, &szDtor);
    TEST_ASSERT(szDtor ==  sizeInBytes, "Falha na execução do dtor")

    obj1 = TObject_Create(&memmgr, sizeInBytes, dtor1);
    onerror(obj1) {
        SHOWERROR("mezza_tests.object_tests._object_all",
                "Falha de criação de objeto")
        return;
    }
    szDtor = 0;
    TObject_Destroy(obj1, &szDtor);
    TEST_ASSERT(szDtor ==  sizeInBytes, "Falha na execução do dtor")

    obj1 = TObject_Create(&memmgr, sizeInBytes, dtor1);
    onerror(obj1) {
        SHOWERROR("mezza_tests.object_tests._object_all",
                "Falha de criação de objeto")
        return;
    }
    szDtor = 0;
    TObject_DestroyByPassDtor(obj1, &szDtor);
    TEST_ASSERT(szDtor ==  0, "Falha na execução sem dtor")

    res = TObject_Exists(&memmgr, obj1);
    TEST_ASSERT(res ==  FALSE, "Detecção de inexistência")

}

TEST_IMPL_TEST(object_all) {
    TEST_CALL_TEST(_object_all)
}
