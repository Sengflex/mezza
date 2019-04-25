/*
 * list_tests.c
 *
 *  Created on: 11 de dez de 2018
 *      Author: root
 */

#include <mezza/SimpleTest.h>
#include <mezza/base/TList.h>
#include <mezza/OnError.h>

TEST_MODULE

static int finder1(TLstNod *node, void *extra) {
    if(*((int *)node->item) == *((int *)extra))
        return 1;
    return 0;
}

static int nodecbk1(TLstNod *node, void *extra) {
    *((int *)extra) += *((int *)node->item);
    return 0;
}

TEST_IMPL_TEST(_list_all) {
    TList *list1;
    TMemMgr memmgr;
    int item1 = 100;
    int item2 = 200;
    int item3 = 300;
    int item4 = 400;
    int sum1=0;
    TLstNod *node1;

    list1 = TList_Create(TMemMgr_Init(&memmgr));
    TEST_ASSERT_FATAL(list1, "Falha de criação")

    TEST_ASSERT_FATAL(&item1 == TList_Add(list1, &item1), "Falha de adição")
    TList_Add(list1, &item2);
    TList_Add(list1, &item3);
    TList_Add(list1, &item4);
    TEST_ASSERT(list1->size==4, "Tamanho deveria ser 4")
    node1 = TList_Get(list1, &item4);
    TEST_ASSERT_FATAL(node1, "Falha de obtenção de nó")
    TEST_ASSERT(node1->item == &item4, "Falha de obtenção de nó. Item errado")

    node1 = TList_GetAt(list1, 0);
    TEST_ASSERT_FATAL(node1, "Falha de obtenção de nó")
    TEST_ASSERT(node1->item == &item1, "Falha de obtenção de nó. Item errado")

    node1 = TList_GetAt(list1, 1);
    TEST_ASSERT_FATAL(node1, "Falha de obtenção de nó")
    TEST_ASSERT(node1->item == &item2, "Falha de obtenção de nó. Item errado")

    node1 = TList_GetAt(list1, 2);
    TEST_ASSERT_FATAL(node1, "Falha de obtenção de nó")
    TEST_ASSERT(node1->item == &item3, "Falha de obtenção de nó. Item errado")

    node1 = TList_GetAt(list1, 3);
    TEST_ASSERT_FATAL(node1, "Falha de obtenção de nó")
    TEST_ASSERT(node1->item == &item4, "Falha de obtenção de nó. Item errado")

    node1 = TList_Find(list1, finder1, &item3);
    TEST_ASSERT_FATAL(node1, "Falha de obtenção de nó")
    TEST_ASSERT(node1->item == &item3, "Falha de obtenção de nó. Item errado")

    TEST_ASSERT_FATAL(0 == TList_Foreach(list1, nodecbk1, &sum1), "Falha de retorno")
    TEST_ASSERT(sum1 == 1000, "Falha de foreach")

    TEST_ASSERT(4 == TList_CountNodes(list1), "Falha de contagem")

    node1 = TList_Get(list1, &item3);
    PROTECT(node1,
            "mezza_tests.list_tests._list_all",
            "Obtenção de objeto do qual dependem os testes a seguir",
            return;)
    TEST_ASSERT(node1 = TList_Rem(list1, node1), "Falha de remoção. Em retornar próximo nó")
    TEST_ASSERT(node1->item == &item4, "Falha de remoção. Nó retornado não é próximo nó")
    TEST_ASSERT(NULL == TList_Get(list1, &item3), "Falha de remoção. Item nao deveria mais ser encontrado")

    TEST_ASSERT(node1->prev->item == &item2, "Falha de remoção. Ligação de nós corrompida")

    node1 = TList_Get(list1, &item1);
    PROTECT(node1,
            "mezza_tests.list_tests._list_all",
            "Obtenção de objeto do qual dependem os testes a seguir",
            return;)
    TEST_ASSERT(node1 = TList_Rem(list1, node1), "Falha de remoção. Em retornar próximo nó")
    TEST_ASSERT(node1->item == &item2, "Falha de remoção. Nó retornado não é próximo nó")
    TEST_ASSERT(list1->size==2, "Tamanho agora deveria ser 2")
    TEST_ASSERT(list1->start == node1, "Falha de remoção. Ligação de nós corrompida")

    node1 = TList_Get(list1, &item4);
    PROTECT(node1,
            "mezza_tests.list_tests._list_all",
            "Obtenção de objeto do qual dependem os testes a seguir",
            return;)
    TEST_ASSERT(NULL == TList_Rem(list1, node1), "Falha de remoção de último item. Em retornar próximo NULL")
    TEST_ASSERT(list1->end->item == &item2, "Falha de remoção de último item. Lista deveria conter apenas o item2")


}

TEST_IMPL_TEST(list_all) {
    TEST_CALL_TEST(_list_all)
}

