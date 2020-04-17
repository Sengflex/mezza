/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * SimpleTest.h
 *
 *  Created on: before v0.2.0
 *      Author: danilo
 */

#ifndef MZ_SIMPLETEST_H_INCLUDED
#define MZ_SIMPLETEST_H_INCLUDED

#include <stdio.h>
#include <string.h>

#define TEST_TOTAL_VAR   fStTotal

#define TEST_PASSED_VAR  fStPassed

#define TEST_SUMMARY fprintf(stderr, "\n================================================================================\n\
[Resumo dos testes: %.2f%% do testes tiveram sucesso]\n\tTotal:    %.0f\n\tPassaram: %.0f\n\
================================================================================\n", TEST_PASSED_VAR / TEST_TOTAL_VAR * 100, TEST_TOTAL_VAR, TEST_PASSED_VAR);

#define TEST_INIT(LABEL) float TEST_TOTAL_VAR = 0.0, TEST_PASSED_VAR = 0.0; fprintf(stderr, "================================================================================\n\
[Suite de Testes para " #LABEL "]\n");

#define TEST_DECL_TEST(TNAME) void TNAME (float * TEST_TOTAL_VAR, float * TEST_PASSED_VAR);

#define TEST_USE_TEST(TNAME) extern void TNAME (float * TEST_TOTAL_VAR, float * TEST_PASSED_VAR);

#define TEST_IMPL_TEST(TNAME) void TNAME (float * TEST_TOTAL_VAR, float * TEST_PASSED_VAR)

#define TEST_ADD_TEST (*TEST_TOTAL_VAR)++;

#define TEST_PASS (*TEST_PASSED_VAR)++;

#define TEST_SWITH_SHOW_TOTAL gShowTestNumber = gShowTestNumber ? 0 : 1;

#define DAS(DII) #DII

#define TEST_SHOW_TOTAL if(gShowTestNumber) fprintf(stderr,  "%d ", (int)*TEST_TOTAL_VAR);

#define TEST_ASSERT(COND, MSG) { TEST_ADD_TEST TEST_SHOW_TOTAL if(!(COND)) { fprintf(stderr, "\n\tTeste falhou: %s\n\tLocal: %s(%d)\n", MSG, __FILE__, __LINE__); } else { TEST_PASS } }

#define TEST_ASSERT_FATAL(COND, MSG) { TEST_ADD_TEST TEST_SHOW_TOTAL if(!(COND)) { fprintf(stderr, "\n\tFATAL! Teste falhou: %s\n\tLocal: %s(%d)\nSaindo da Unidade...", MSG, __FILE__, __LINE__); return; } else { TEST_PASS } }

#define TEST_CHECK_PREPARATION(COND) if(!(COND)) { fprintf(stderr, "\nFalha de preparação. Abortando...\n\t\tLocal: %s(%d)\n", __FILE__, __LINE__); return; }

#define TEST_CALL_TEST(TEST) fputs("\n    TEST " #TEST "\n", stderr); TEST(TEST_TOTAL_VAR, TEST_PASSED_VAR);

#define TEST_CALL_TEST_MODULE(MODULE) fputs("\nMODULO -------------------------------------------------------------------------\n\
  " #MODULE, stderr); MODULE(& TEST_TOTAL_VAR, & TEST_PASSED_VAR);

#define TEST_MAIN int gShowTestNumber = 0;
#define TEST_MODULE extern int gShowTestNumber;

#endif /* MZ_SIMPLETEST_H_INCLUDED */
