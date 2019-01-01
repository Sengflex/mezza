/*
 ============================================================================
 Name        : mezza_tests.c
 Author      : Danilo Almeida
 Version     :
 Copyright   : Copyright 2018 Danilo Marcos Pontes de Almeida. All rights reserved.
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include <mezza/SimpleTest.h>
#include <mezza/base/Exceptions.h>

TEST_MAIN

TEST_USE_TEST(mem_all)
TEST_USE_TEST(excptions_all)
TEST_USE_TEST(object_all)
TEST_USE_TEST(list_all)
TEST_USE_TEST(str_all)
TEST_USE_TEST(file_all)
TEST_USE_TEST(dbase_all)
TEST_USE_TEST(collections_all)
TEST_USE_TEST(conf_all)

int main2();

int main() {
    TEST_INIT(Mezza Library)

    TEST_SWITH_SHOW_TOTAL

    TEST_CALL_TEST_MODULE(mem_all)
    TEST_CALL_TEST_MODULE(excptions_all)
    TEST_CALL_TEST_MODULE(object_all)
    TEST_CALL_TEST_MODULE(list_all)
    TEST_CALL_TEST_MODULE(str_all)
    TEST_CALL_TEST_MODULE(file_all)
    TEST_CALL_TEST_MODULE(dbase_all)
    TEST_CALL_TEST_MODULE(collections_all)
    TEST_CALL_TEST_MODULE(conf_all)

    TEST_SUMMARY

    main2();
    
    return 0;
}


int function3() {
    throw(Exception, 1)

    return 0;
}

int function2() {
    throw(ExceptionUnknown, 1)

    return 0;
}

int function() {
    function2();
    throw(Exception, 1)

    return 0;
}

int teste1() {

    try(teste1)
        function(); check(teste1)

        puts("Tudo OK!");

    catch(teste1)
        on(Exception)
            /* Tratamento daqui */
            //print_exceptions();
            printf("Tratamento de Exception bem sucedido...\n");
            //throw(ExceptionBanana,1)
            /* até auqi */
        end_on
    end_try(teste1)

    return 0;
}

/*

try_continue(ID) ou try(ID)
    <<chamada de funcao>> check(ID)

do_continue(ID) <------|
                    | obrigatório com try_continue
end_continue(ID) <--|

catch(ID)
    on(Exception)

    end_on
end_try(ID)



 */

int main2() {

    try_continue(main)
        teste1(); check(main)

        puts("Tudo OK!");

    do_continue(main)
        puts("Execução do continue");
    end_continue(main)

    catch(main)
        on(ExceptionBanana)
            /* Tratamento daqui */
            //print_exceptions();
            printf("Tratando exceção desconhecida...\n");
            Exceptions_PrintException(NULL);
            /* até auqi */
        end_on
    end_try(main)

    return 0;
}
