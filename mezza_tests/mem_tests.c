

#include <mezza/SimpleTest.h>
#include <mezza/base/TMemMgr.h>

TEST_MODULE

TEST_IMPL_TEST(_mem_all) {
    TMemMgr mlist;
    void *addr1=NULL;
    TSize size1 = 100;

    /* -> Solução: Tarefa 1, Fase 1  */
    {
        mlist.start         = 1;
        mlist.end           = 2;
        mlist.nofAlocated   = 3;

        TMemMgr_Init(&mlist);

        TEST_ASSERT(mlist.start == NULL, "campo start deveria ser NULL")
        TEST_ASSERT(mlist.end == NULL, "campo end deveria ser NULL")
        TEST_ASSERT(mlist.nofAlocated == 0, "campo nofAlocated deveria ser 0")
    }
    /* <- Solução: Tarefa 1, Fase 1  */

    /* -> Solução: Tarefa 1, Fase 2  */
        addr1 = TMemMgr_Alloc(&mlist, size1);
        TEST_ASSERT_FATAL(addr1, "Falha de alocação")
    /* <- Solução: Tarefa 1, Fase 2  */

    /* -> Solução: Tarefa 1, Fase 6  */
    {
        TBool res;

        res = TMemMgr_Exists(&mlist, addr1);
        TEST_ASSERT(res == TRUE, "Falha de detecçaõ")
    }
    /* <- Solução: Tarefa 1, Fase 6  */

    /* -> Solução: Tarefa 1, Fase 7  */
    {
        TSize res;

        res = TMemMgr_SizeOf(&mlist, addr1);
        TEST_ASSERT(res == size1, "Falha de meta dados (tamanho)")
    }
    /* <- Solução: Tarefa 1, Fase 7  */

    /* -> Solução: Tarefa 1, Fase 3  */
    {
        TSize res;
        TSize size2 = size1 * 2;
        void *addr2;

        addr2 = TMemMgr_Realloc(&mlist, addr1, size2);
        TEST_ASSERT_FATAL(addr2, "Falha de realocação")
        res = TMemMgr_SizeOf(&mlist, addr2);
        TEST_ASSERT(res == size1 * 2, "Falha de meta dados (tamanho)")
    }
    /* <- Solução: Tarefa 1, Fase 3  */

    /* -> Solução: Tarefa 1, Fase 4  */
    {
        TBool res;
        void *addr2;

        addr2 = TMemMgr_Free(&mlist, addr1);
        TEST_ASSERT(addr2==NULL, "Falha de retorno")
        res = TMemMgr_Exists(&mlist, addr1);
        TEST_ASSERT(res == FALSE, "Falha de detecçaõ")
    }
    /* <- Solução: Tarefa 1, Fase 4  */

    /* -> Solução: Tarefa 1, Fase 5  */
    {
        TBool res;
        void *addr3;
        void *addr4;
        void *addr5;

        addr3 = TMemMgr_Alloc(&mlist, size1);
        TEST_ASSERT_FATAL(addr3, "Falha de alocação")
        addr4 = TMemMgr_Alloc(&mlist, size1);
        TEST_ASSERT_FATAL(addr4, "Falha de alocação")
        addr5 = TMemMgr_Alloc(&mlist, size1);
        TEST_ASSERT_FATAL(addr5, "Falha de alocação")

        TMemMgr_FreeAll(&mlist);
        res = TMemMgr_Exists(&mlist, addr3);
        TEST_ASSERT(res == FALSE, "Falha de detecçaõ")
        res = TMemMgr_Exists(&mlist, addr4);
        TEST_ASSERT(res == FALSE, "Falha de detecçaõ")
        res = TMemMgr_Exists(&mlist, addr5);
        TEST_ASSERT(res == FALSE, "Falha de detecçaõ")
    }
    /* <- Solução: Tarefa 1, Fase 5  */
}

TEST_IMPL_TEST(mem_all) {
    TEST_CALL_TEST(_mem_all)
}
