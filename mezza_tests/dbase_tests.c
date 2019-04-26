/*
 * dbase_sqlite3conn_tests.c
 *
 *  Created on: 12 de dez de 2018
 *      Author: root
 *
 */

#include <mezza/dbase/TDBType.h>
#include <mezza/dbase/TSQLite3Conn.h>
#include <mezza/dbase/TDBRow.h>
#include <mezza/dbase/TDBTable.h>
#include <mezza/dbase/TDBField.h>
#include <mezza/FileUtil.h>
#include <mezza/base/TObject.h>
#include <mezza/base/TMemMgr.h>
#include <mezza/base/TList.h>
#include <mezza/SimpleTest.h>

TEST_MODULE

TEST_IMPL_TEST(_dbase_conn) {
}

TEST_IMPL_TEST(_dbase_row) {
    TMemMgr memmgr;
    TString id=NULL, desc=NULL, unit=NULL, price=NULL;
    char *cells[4] = { "1", "Luva de borracha", "par", "3.00" };
    TSize cellsSz = 4;
    char *cells2[4] = { "2", "Luva de borracha", "cx", "100.00" };
    TSize cells2Sz = 4;
    TDBRow *row;
    TStatus res;

    row = TDBRow_Create(TMemMgr_Init(&memmgr), cells, cellsSz);
    TEST_ASSERT(row, "Falha de criação de objeto TDBRow")

    res = TDBRow_Get(&memmgr, row, 4, &id, &desc, &unit, &price);
    TEST_ASSERT(OK == res, "Falha de retorno")
    TEST_ASSERT(0 == strcmp(id, cells[0]), "Falha de conteudo")
    TEST_ASSERT(0 == strcmp(desc, cells[1]), "Falha de conteudo")
    TEST_ASSERT(0 == strcmp(unit, cells[2]), "Falha de conteudo")
    TEST_ASSERT(0 == strcmp(price, cells[3]), "Falha de conteudo")

    res = TDBRow_Set(&memmgr, row, cells2Sz, cells2[0], cells2[1], cells2[2], cells2[3]);
    TEST_ASSERT(OK == res, "Falha de retorno")

    id = TObject_Destroy(id, NULL);
    desc = TObject_Destroy(desc, NULL);
    unit = TObject_Destroy(unit, NULL);
    price = TObject_Destroy(price, NULL);
    res = TDBRow_Get(&memmgr, row, cells2Sz, &id, &desc, &unit, &price);
    TEST_ASSERT(OK == res, "Falha de retorno")
    TEST_ASSERT(0 == strcmp(id, cells2[0]), "Falha de conteudo")
    TEST_ASSERT(0 == strcmp(desc, cells2[1]), "Falha de conteudo")
    TEST_ASSERT(0 == strcmp(unit, cells2[2]), "Falha de conteudo")
    TEST_ASSERT(0 == strcmp(price, cells2[3]), "Falha de conteudo")

	TMemMgr_FreeAll(&memmgr);
}

static int finderName(TDBRow *row, void *data) {
	TLstNod *node = row->cells->start->next;

	if(strstr((char *)node->item, (char*)data))
		return 1;

	return 0;
}

TEST_IMPL_TEST(_dbase_table) {
    TMemMgr  memmgr;
    char *filename = "mezza.dbase.002.db";
    TSQLite3Conn *conn;
    char *tableName = "pessoas";
    TDBTable *table;
    TDBField fields[3] = {
            {"id", eINT, PRIMARY_KEY | AUTO_INC, NULL},
            {"nome", eTEXT, NOT_NULL, NULL},
            {"idade", eINT, DEFAULT_VAL, "0"}
    };
    TSize fieldsCount = 3;
    TStatus status;
    char *registerContent1[3] = { "0", "Marcos Afonso", "85" };
    char *registerContent2[3] = { "1", "Antonio Moraes", "74" };
    char *registerContent3[3] = { "2", "Fidelina Castro", "45" };
    char *registerContent4[3] = { "3", "Mozina Aze", "67" };
    char *registerContent5[3] = { "4", "Anabele Muniz", "50" };
    TDBRow *register1[5] = { NULL,NULL,NULL,NULL,NULL };
    int i=0, j=0;
    TDBRow *registroObtido;
    TLstNod *cellObtida, *cellOrigem;
    TDBRow *rowOfMozina=NULL, *rowOfMoraes=NULL;

    /* Realiza uma conexão no novo banco de dados */
    conn = TSQLite3Conn_Create(TMemMgr_Init(&memmgr), filename);
    TEST_ASSERT(conn, "Criação de objeto conexão")
    PROTECT(conn, "\n\t TEST ERROR", "", remove(filename); return;)
    status = TSQLite3Conn_Connect(conn);    
    TEST_ASSERT(status==OK, "Ativação da conexão")
    PROTECT(conn, "\n\t TEST ERROR", "",  TObject_Destroy(conn, NULL); remove(filename); return;)

    /* Cria uma nova tabeça no banco de dados */
    table = TDBTable_Create(&memmgr, conn, tableName, fields, fieldsCount);
    TEST_ASSERT(table, "Criação de objeto tabela")

    /* Adiciona dados */
    register1[0] = TDBRow_Create(&memmgr, registerContent1, fieldsCount);
    TEST_CHECK_PREPARATION(register1[0])
    register1[1] = TDBRow_Create(&memmgr, registerContent2, fieldsCount);
    TEST_CHECK_PREPARATION(register1[1])
    register1[2] = TDBRow_Create(&memmgr, registerContent3, fieldsCount);
    TEST_CHECK_PREPARATION(register1[2])
    register1[3] = TDBRow_Create(&memmgr, registerContent4, fieldsCount);
    TEST_CHECK_PREPARATION(register1[3])
    register1[4] = TDBRow_Create(&memmgr, registerContent5, fieldsCount);
    TEST_CHECK_PREPARATION(register1[4])

    TEST_ASSERT_FATAL(OK==DBTable_AddRow(table, register1[0]), "Adicao de dados a tabela")
	TEST_ASSERT_FATAL(OK==DBTable_AddRow(table, register1[1]), "Adicao de dados a tabela")
	TEST_ASSERT_FATAL(OK==DBTable_AddRow(table, register1[2]), "Adicao de dados a tabela")
	TEST_ASSERT_FATAL(OK==DBTable_AddRow(table, register1[3]), "Adicao de dados a tabela")
	TEST_ASSERT_FATAL(OK==DBTable_AddRow(table, register1[4]), "Adicao de dados a tabela")

	TEST_ASSERT_FATAL(OK==DBTable_WriteChanges(table), "Escrita dos dados da tabela")

    PROTECT(table, "\n\t TEST ERROR", "", TObject_Destroy(conn, NULL); remove(filename); return;)
    table = TObject_Destroy(table, NULL);

    /* Abre a tabela criada anteriormente */
    table = TDBTable_Load(&memmgr, conn, tableName);
    TEST_ASSERT_FATAL(table, "Carregamento de tabela")

    TEST_ASSERT(table->rowsCount==5, "Campo rowsCount deveria ser NULL")
    TEST_ASSERT(table->fieldsCount==fieldsCount, "Campo fieldsCount deveria ser fieldsCount")
    TEST_ASSERT(table->availableId==5, "Campo availableId deveria ser 0")
    TEST_ASSERT(table->isLoaded==TRUE, "Campo isLoaded deveria ser TRUE")
    TEST_ASSERT_FATAL(table->conn, "Campo conn deveria ser diferente de NULL")
    TEST_ASSERT_FATAL(table->name, "Campo conn deveria ser diferente de NULL")
    TEST_ASSERT(0==strcmp(table->name, tableName), "Campo name com valor errado")
    TEST_ASSERT_FATAL(table->rows, "Campo rows deveria ser diferente de NULL")
    TEST_ASSERT(table->rows->size==5, "Campo rows deveria conter 5 itens")
    TEST_ASSERT_FATAL(table->fields, "Campo rows deveria ser diferente de NULL")
    TEST_ASSERT(table->fields->size==fieldsCount, "Campo fields deveria ser uma lista com fieldsCount elementos")

    /* Adiciona dados */
    register1[0] = TDBRow_Create(&memmgr, registerContent1, fieldsCount);
    TEST_CHECK_PREPARATION(register1[0])
    register1[1] = TDBRow_Create(&memmgr, registerContent2, fieldsCount);
    TEST_CHECK_PREPARATION(register1[1])
    register1[2] = TDBRow_Create(&memmgr, registerContent3, fieldsCount);
    TEST_CHECK_PREPARATION(register1[2])
    register1[3] = TDBRow_Create(&memmgr, registerContent4, fieldsCount);
    TEST_CHECK_PREPARATION(register1[3])
    register1[4] = TDBRow_Create(&memmgr, registerContent5, fieldsCount);
    TEST_CHECK_PREPARATION(register1[4])

	LOOPLIST(table->rows,)
		/* Cada registro obtido */
		registroObtido = (TDBRow *)_NODE_->item;
		cellObtida = registroObtido->cells->start;
		cellOrigem = register1[i]->cells->start;
		while(cellObtida) {
			TEST_ASSERT(0==strcmp((char*)cellObtida->item, (char*)cellOrigem->item), "Diferença de conteudo em células")
			cellObtida = cellObtida->next;
			cellOrigem = cellOrigem->next;
			j++;
		}
			/* Cada celula do registro */

		i++;
	END_LOOPLIST

	TEST_ASSERT(15==j, "Execucoes de laço erradas")
	TEST_ASSERT(5==i, "Execucoes de laço erradas")

	rowOfMozina = DBTable_GetRowById(table, "3");
	TEST_ASSERT_FATAL(rowOfMozina, "Falha em obter registro pelo id")
	cellObtida=rowOfMozina->cells->start->next;
	TEST_ASSERT(0==strcmp((char *)cellObtida->item, registerContent4[1]), "Erro de conteudo para mozina")
	cellObtida=cellObtida->next;
	TEST_ASSERT(0==strcmp((char *)cellObtida->item, registerContent4[2]), "Erro de conteudo para mozina")

	rowOfMoraes = DBTable_GetRowByFinder(table, finderName, "Moraes");
	TEST_ASSERT_FATAL(rowOfMoraes, "Falha em obter registro pelo finder")
	cellObtida=rowOfMoraes->cells->start->next;
	TEST_ASSERT(0==strcmp((char *)cellObtida->item, registerContent2[1]), "Erro de conteudo para Moraes")
	cellObtida=cellObtida->next;
	TEST_ASSERT(0==strcmp((char *)cellObtida->item, registerContent2[2]), "Erro de conteudo para Moraes")

	TEST_ASSERT_FATAL(OK==DBTable_DelRow(table, rowOfMoraes), "Falha de retorno")
	TEST_ASSERT_FATAL(OK==DBTable_WriteChanges(table), "Escrita dos dados da tabela")

	table = TObject_Destroy(table, NULL);

    /* Abre a tabela criada anteriormente */
    table = TDBTable_Load(&memmgr, conn, tableName);
    TEST_CHECK_PREPARATION(table)

    TEST_ASSERT(table->rowsCount==4, "Campo rowsCount deveria ser 4")
	rowOfMoraes = DBTable_GetRowByFinder(table, finderName, "Moraes");
	TEST_ASSERT_FATAL(NULL==rowOfMoraes, "Moraes nao deveria mais ser encontrado")



	table = TObject_Destroy(table, NULL);
	conn = TObject_Destroy(conn, NULL);
	TMemMgr_FreeAll(&memmgr);
    remove(filename);
}

TEST_IMPL_TEST(dbase_all) {
    TEST_CALL_TEST(_dbase_row)
    TEST_CALL_TEST(_dbase_table)
}
