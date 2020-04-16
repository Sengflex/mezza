
#include <stdio.h>
#include <mezza/SimpleTest.h>
#include <mezza/base/Exceptions.h>

TEST_MODULE

void _excptions_all_aux1() {
    throw(Exception, )
}

void _excptions_all_aux2() {
    throw(ExceptionBanana, )
}

TEST_IMPL_TEST(_excptions_all) {
    int passed=0;
    int passed1=0;
    int passed2=0;
    int passed3=0;
    int passed4=0;
    int passed5=0;

    try(id_001)
        _excptions_all_aux1(); check(id_001)
    catch(id_001)
        on(Exception)
            passed=1;
        end_on
    end_try(id_001)

    try_continue(id_005)
        _excptions_all_aux1(); check(id_005)

    do_continue(id_005)
            passed4=1;
    end_continue(id_005)
    catch(id_005)
        on(Exception)

        end_on
    end_try(id_005)

    try_continue(id_006)
        throw_local(id_006, Exception)

    do_continue(id_006)
            passed5=1;
    end_continue(id_006)
    catch(id_006)
        on(Exception)

        end_on
    end_try(id_006)

    try(id_002)
        throw_local(id_002, Exception)
    catch(id_002)
        on(Exception)
            passed1=1;
        end_on
    end_try(id_002)

    try(id_003)
        _excptions_all_aux2(); check(id_003)
    catch(id_003)
        on(ExceptionBanana)
            passed2=1;
        end_on
        on(Exception)

        end_on
    end_try(id_003)

    try(id_004)
        _excptions_all_aux2(); check(id_004)
    catch_quickly(id_004)
            passed3=1;
    end_try(id_004)

    TEST_ASSERT(passed ==1, "Falha de captura de exceção")
    TEST_ASSERT(passed1==1, "Falha de captura de exceção")
    TEST_ASSERT(passed2==1, "Falha de captura de exceção")
    TEST_ASSERT(passed3==1, "Falha de captura de exceção")
    TEST_ASSERT(passed4==1, "Falha de captura de exceção")
    TEST_ASSERT(passed5==1, "Falha de captura de exceção")
}

TEST_IMPL_TEST(excptions_all) {
    TEST_CALL_TEST(_excptions_all)
}