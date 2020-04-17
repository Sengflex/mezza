
/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * dbase/TDBTable.h
 *
 *  Created on:  22 de Novembro de 2018, 15:22
 *      Author: Danilo Almeida
 */

#include <stdio.h>
#include <string.h>

#define MZ_DBASE_DBTABLE_IMPLEMENT
#include "TDBTable.h"

#include "TDBRow.h"
#include "TDBField.h"
#include "TSQLite3Conn.h"
#include "../OnError.h"
#include "../base/TList.h"
#include "../Lang.h"
#include "../base/TObject.h"
#include "../str/StringBuilder.h"
#include "../base/Exceptions.h"

/*
 * File:   dbtable.c
 * Author: root
 *
 * Created on 22 de Novembro de 2018, 15:22
 */

extern TString row_to_sql_insert(TDBRow *row, TDBTable *table);
extern TString row_to_sql_update(TDBRow *row, TDBTable *table);
extern TString row_to_sql_delete(TDBRow *row, TDBTable *table);

/* módulo TDBField */
extern TDBFieldType str2fieldType(char *str);

static int  table_sqlite_callback(void *data, int argc, char **argv, char **azColName);
static void table_objdtor(TObject addr, void *ignore);

static TStatus   dbtable_delrow_aux   (TDBTable *table, TDBRow *row, TBool ignoreVerif);

/* #include "TDBTable_aux.inc" */
/**********************************************************************************************************************************/
static TString field2sql(TMemMgr *memlist, TDBField *field) {
    TString sql_ret=NULL;
    char *type;

    try(field2sql_try001)
		 sql_ret = TString_Create(memlist, NULL, 300); check(field2sql_try001)

		 TString_CopyF(&sql_ret, "'%s' ", field->name); check(field2sql_try001)

		 switch(field->type) {
		 case eCHAR:
		 case eTEXT:
			 type = "TEXT";
			 break;
		 case eSHORT:
		 case eINT:
		 case eLONG:
			 type = "INTEGER";
			 break;
		 case eFLOAT:
		 case eDOUBLE:
			 type = "REAL";
			 break;
		 case eBUFFER:
			 type = "BLOB";
			 break;
		 }

		 TString_ConcatF(&sql_ret, "%s", type); check(field2sql_try001)

		 if(PRIMARY_KEY & field->contraints)
			 TString_Concat(&sql_ret, " PRIMARY KEY"); check(field2sql_try001)
		 else {
			 if(NOT_NULL & field->contraints)
				 TString_Concat(&sql_ret, " NOT NULL"); check(field2sql_try001)
			 if(DEFAULT_VAL & field->contraints)
				 TString_ConcatF(&sql_ret, " DEFAULT %s", field->defaultVal); check(field2sql_try001)
		 }
		 if(AUTO_INC & field->contraints)
			 TString_Concat(&sql_ret, " AUTOINCREMENT"); check(field2sql_try001)

	catch(field2sql_try001)
		on(Exception)
			throw_note(ExceptionTableInternals, NULL, "Field to SQL conversion")
		end_on
	end_try(field2sql_try001)

    return sql_ret;
}

static TString gen_create_table_sql(TMemMgr *memmgr, TDBTable *table) {
    TString sql_ret=NULL;
    TString fieldAsSql=NULL;

    try(gen_create_table_sql_try001)
		sql_ret = TString_Create(memmgr, NULL, 300); check(gen_create_table_sql_try001)

		TString_CopyF(&sql_ret, "CREATE TABLE '%s' (", table->name); check(gen_create_table_sql_try001)

		LOOPLIST(table->fields,)
			fieldAsSql = field2sql(memmgr, _NODE_->item);  check(gen_create_table_sql_try001)
			TString_ConcatF(&sql_ret, "%s%s", fieldAsSql,
					_NODE_ == table->fields->end ? "" : ","); check(gen_create_table_sql_try001)

			fieldAsSql = TObject_Destroy(fieldAsSql, NULL);
		END_LOOPLIST

		TString_Concat(&sql_ret, ");"); check(gen_create_table_sql_try001)

	catch(gen_create_table_sql_try001)
		on(Exception)
			if(sql_ret) {
				TObject_Destroy(sql_ret, NULL);

				if(fieldAsSql)
					TObject_Destroy(fieldAsSql, NULL);
			}
			throw_note(ExceptionTableInternals, NULL, "Composition of 'create table' SQL command")
		end_on
	end_try(gen_create_table_sql_try001)

    return sql_ret;
}

static TDBTable *alloc_table(TMemMgr *memmgr, TSQLite3Conn *conn, char *name, TDBField *vfields, TCount vfieldsCount) {
    TDBTable *ret = NULL;
    TDBField *newField=NULL;

    try(alloc_table)
             /* Cria o objeto tabela */
        ret = TObject_CreateZero(memmgr, sizeof(TDBTable), table_objdtor); check(alloc_table)

        if(vfields) {
            ret->fields = TList_Create(memmgr);
            check_note(alloc_table, "Criação da lista de campos")

            loop_i(vfieldsCount)
                newField = DBField_Create(memmgr,
                                            vfields[_INDEX_].name,
                                            vfields[_INDEX_].type,
                                            vfields[_INDEX_].contraints,
                                            vfields[_INDEX_].defaultVal);
            	check_note(alloc_table, "Adicao de campo à lista")

                TList_Add(ret->fields, newField);
            	check_note(alloc_table, "Adicao de campo à lista")
            	newField = NULL; /* Não mexa no que não entende :-) */
            end_loop_i
        } else {
            ret->fields = NULL;
        }
        /* Cria um objeto string para conter o nome da tabela */
        ret->name = TString_Create(memmgr, name, 0);
    	check_note(alloc_table, "Atribuicao do nome da tabela")

        /* Cria a lista que conterá os registros obtidos do banco de dados */
		ret->rows = TList_Create(memmgr);
    	check_note(alloc_table, "Criacao de lista de registros")

        /* Completa ainicialização */
        if(vfields)
            ret->fieldsCount = vfieldsCount;
        else
            ret->fieldsCount = 0;
        ret->rowsCount = 0;
        ret->isLoaded = FALSE;
        ret->conn = conn;
        ret->availableId = 0; /* No carregamento, o id disponível é calculado */

    catch(alloc_table)
        on(Exception)
			if(ret) {
				if(ret->fields) {
					TList_ForeachDoDestroy(ret->fields, NULL);
					ret->fields = TObject_Destroy(ret->fields, NULL);
				}

				if(newField)
					TObject_Destroy(newField, NULL);

				if(ret->name)
					TObject_Destroy(ret->name, NULL);

				TObject_DestroyByPassDtor(ret, NULL);
			}
            throw_note(ExceptionDBTableAllocation, NULL, _TRY_NOTE_)
		end_on
    end_try(alloc_table)

    return ret;
}


static int table_scheme_sqlite_callback(void *data, int argc, char **argv, char **azColName) {
    TString *result = (TString *)data;

    onerror(TString_Copy(result, argv[0], 500))
    	throw_note(ExceptionTableInternals, 1, "Callback SQLite3 para Obtençao do esquema da tabela")

    return 0;
}

static TStatus parse_field_constraints(TMemMgr *memmgr, char *constraintsStr, unsigned int *constraints, TString *defaultVal) {
    char *whereisDefaultConst;
    unsigned int local_constraints=0;
    TString local_defaultVal=NULL;

    if(strstr(constraintsStr, "PRIMARY KEY"))
        local_constraints |= PRIMARY_KEY;
    if(strstr(constraintsStr, "NOT NULL"))
        local_constraints |= NOT_NULL;
    if(strstr(constraintsStr, "UNIQUE"))
        local_constraints |= UNIQUE;
    if(strstr(constraintsStr, "AUTOINCREMENT"))
        local_constraints |= AUTO_INC;
    if((whereisDefaultConst=strstr(constraintsStr, "DEFAULT"))) {
        char *endpos;
        TCount diff;
        local_constraints |= DEFAULT_VAL;
        whereisDefaultConst = &whereisDefaultConst[8];
        if(*whereisDefaultConst=='\'') {

            whereisDefaultConst++;
            endpos = strstr(whereisDefaultConst, "'");
            onerror( endpos )
            	throw_note(ExceptionTableInternals, FAIL, "Erro de definicao ou sintaxe das restricoes de campo (field constraints). Era esperado um terminador de aspas simples após DEFAULT")

            diff = (endpos - whereisDefaultConst)/sizeof(char);
        } else {

            endpos = strstr(whereisDefaultConst, " ");
            if(!endpos)
                diff = strlen(whereisDefaultConst);
            else
                diff = (endpos - whereisDefaultConst)/sizeof(char);
        }
        local_defaultVal = TString_Create(memmgr, whereisDefaultConst, diff);
    }

    *constraints = local_constraints;
    *defaultVal  = local_defaultVal;

    return OK;
}

static TStatus load_table_scheme(TDBTable *table) {
    TMemMgr *memmgr            = TObject_ManagerOf(table);
    TString  schemeQueryResult = NULL,
             discard           = NULL,
             fieldsScheme      = NULL;
    TList   *tmpFieldList;

    try(load_table_scheme_try001)


		 {
			 TString sqlToGetScheme = TString_Create(memmgr, NULL, 200); check(load_table_scheme_try001)

			 TString_CopyF(&sqlToGetScheme, "SELECT sql FROM sqlite_master WHERE type = 'table' and tbl_name = '%s'", table->name); check(load_table_scheme_try001)

			 schemeQueryResult = TString_Create(memmgr, NULL, 500); check(load_table_scheme_try001)

			TSQLite3Conn_ExecSQL(table->conn, sqlToGetScheme, table_scheme_sqlite_callback, &schemeQueryResult); check(load_table_scheme_try001)

			 TObject_Destroy(sqlToGetScheme, NULL);
		 }

		 StringBuilder_FromString(memmgr, schemeQueryResult, "%(%)", &discard, &fieldsScheme); check(load_table_scheme_try001)
		 TObject_Destroy(schemeQueryResult, NULL);
		 TObject_Destroy(discard, NULL);

		 /* Obtem o esquema de cada campo separadamente */
		 tmpFieldList = TList_Create(memmgr); check(load_table_scheme_try001)
		 {
			 TString fieldName          =NULL;
			 TString fieldType          =NULL;
			 TString fieldConstraintsStr=NULL;
			 TString defaultVal         = NULL;
			 unsigned int constraints;
			 TDBField *tmpField;
			 char *curFieldScheme = strtok(fieldsScheme, ",");

			 while(curFieldScheme) {
				StringBuilder_FromString(memmgr, curFieldScheme, "'%' % %", &fieldName, &fieldType, &fieldConstraintsStr); check(load_table_scheme_try001)

				constraints=0;
				parse_field_constraints(memmgr, fieldConstraintsStr, &constraints, &defaultVal); check(load_table_scheme_try001)

				tmpField = DBField_Create(memmgr, fieldName, str2fieldType(fieldType), constraints, defaultVal); check(load_table_scheme_try001)


				 TList_Add(tmpFieldList, tmpField); check(load_table_scheme_try001)

				 fieldName          =TObject_Destroy(fieldName, NULL);
				 fieldType          =TObject_Destroy(fieldType, NULL);
				 fieldConstraintsStr=TObject_Destroy(fieldConstraintsStr, NULL);
				 defaultVal         =TObject_Destroy(defaultVal, NULL);
				 curFieldScheme = strtok(NULL, ",");
			 }
		 }
		 TObject_Destroy(fieldsScheme, NULL);

		 table->fieldsCount = tmpFieldList->size;
		 table->fields = tmpFieldList;
	catch(load_table_scheme_try001)
		on(Exception)
			throw_note(ExceptionTableInternals, FAIL, "Obtencao do esquema SQL da tabela")
		end_on
	end_try(load_table_scheme_try001)


    return OK;
}

static TStatus dbtable_load_table_data(TDBTable *table, char *where) {
    TString  sql=NULL;

    try(load_table_data_try001)
		/* Prepara o código SQL */
		sql = TString_Create(TObject_ManagerOf(table), NULL, 200); check_note(load_table_data_try001, "Formacao do comando SQL")

		TString_CopyF(&sql, "SELECT * FROM %s", table->name); check_note(load_table_data_try001, "Formacao do comando SQL")

		if(where) {
			TString_ConcatF(&sql, " WHERE %s", where); check_note(load_table_data_try001, "Formacao do comando SQL")
		}

		/* Execulta a consulta e pôe os dados numa tabela */
		TSQLite3Conn_ExecSQL(table->conn, sql, table_sqlite_callback, table); check(load_table_data_try001)

		TObject_Destroy(sql, NULL);
		table->isLoaded = TRUE;

		{ /* Atualiza o id disponível */
			TId idAsNumber;

			LOOPLIST(table->rows,)
				idAsNumber = atoi((char *)((TDBRow*)_NODE_->item)->cells->start->item);
				if(idAsNumber >= table->availableId)
					table->availableId = idAsNumber + 1;
			END_LOOPLIST
		}
	catch(load_table_data_try001)
		on(Exception)
			if(sql)
				TObject_Destroy(sql, NULL);
			throw_note(ExceptionTableInternals, FAIL, "Carregamento dos dados da tabela")
		end_on
	end_try(load_table_data_try001)

    return OK;
}


static TStatus dbtable_addrow_aux(TDBTable *table, TDBRow *row, TDBRowChangeType typeOfChange) {

    /* Adiciona registro a tabela */
    onerror( TList_Add(table->rows, row) )
		throw_note(ExceptionTableInternals, FAIL, "Backend de adicao de registro na tabela. Adicao de objeto na tabela de registros")

    row->needChange = typeOfChange;
    row->parentTable = table;
    table->rowsCount++;

    return OK;
}


/*
 * table_sqlite_callback
 *
 * Historico:
 *
 * ...:
 *  Criada.
 *
 * 30-NOV-2018, Danilo:
 *  Terminada.
 */
static int table_sqlite_callback(void *data, int argc, char **argv, char **azColName) {
    TDBTable *table = (TDBTable *)data;

    /* Adiciona o registro (aka linha, row) */
    onerror( OK==dbtable_addrow_aux(table, TDBRow_Create(TObject_ManagerOf(table), argv, argc), eNONE) )
		throw_note(ExceptionTableInternals, 1, "Callback SQLite3 de adicao de registro na tabela")

    return 0;
}

/*
 * table_objdtor
 *
 * Historico:
 *
 * ...:
 *  Criada.
 *
 * 30-NOV-2018, Danilo:
 *  Terminada.
 */
static void table_objdtor(TObject obj, void *ignore) {
    TDBTable *table = (TDBTable *)obj;

    if(table->fields)
        TObject_Destroy(table->fields, NULL);

    if(table->name)
        TObject_Destroy(table->name, NULL);

    LOOPLIST( table->rows, TObject_Destroy((TDBRow *)_NODE_->item, NULL); ) END_LOOPLIST
    TObject_Destroy(table->rows, NULL);
}
/**********************************************************************************************************************************/

/*
 * DBTable_Create
 *
 * Historico:
 *
 * ...:
 *  Criada
 * 25-NOV-2018, Danilo:
 *  Terminada.
 * 29-NOV-2018, Danilo:
 *  Modificada. Agora recebe nome e campos na criação
 * 30-NOV-2018, Danilo:
 *  Documentada.
 */
TDBTable *TDBTable_Create   (TMemMgr *memmgr, TSQLite3Conn *conn, char *name, TDBField *vfields, TCount vfieldsCount)
{
    TDBTable *table = NULL;
    TString create_sql;

    onerror( conn->isConnected == TRUE )
    	throw_note(ExceptionTableCreation, NULL, "A conexao nao esta ativada")

    onerror( 0 == strcmp("id", vfields[0].name))
        throw_note(ExceptionTableCreation, NULL, "O campo id deve sempre ser o primeiro")

    try_continue(table_create_try001)
        table = alloc_table(memmgr, conn, name, vfields, vfieldsCount); check(table_create_try001)

        create_sql = gen_create_table_sql(memmgr, table); check(table_create_try001)

		TSQLite3Conn_ExecSQL(conn, create_sql, table_sqlite_callback, table); check(table_create_try001)

	do_continue(table_create_try001)
		if(create_sql)
			TObject_Destroy(create_sql, NULL);
	end_continue(table_create_try001)

    catch(table_create_try001)
        on(Exception)
			if(table)
				TObject_Destroy(table, NULL);

            throw(ExceptionTableCreation, NULL)
        end_on
    end_try(table_create_try001)

    table->isLoaded = TRUE;

    return table;
}


/*
 * DBTable_Load
 *
 * Historico:
 *
 * 30-NOV-2018, Danilo:
 *  Criada
 * 02-dez-2018, Danilo:
 *  Terminada.
 */
TDBTable *TDBTable_Load(TMemMgr *memmgr, TSQLite3Conn *conn, char *name) {
    return TDBTable_LoadWhere(memmgr, conn, name, NULL);
}

/*
 * DBTable_LoadWhere
 *
 * Historico:
 *
 * ...:
 *  Criada
 * 02-dez-2018, Danilo:
 *  Terminada.
 */
TDBTable   *TDBTable_LoadWhere(TMemMgr *memmgr, TSQLite3Conn *conn, char *name, char *where) {
    TDBTable *table=NULL;

    try(table_loadwhere_try001)

		table = alloc_table(memmgr, conn, name, NULL, 0); check(table_loadwhere_try001)

		load_table_scheme(table); check(table_loadwhere_try001)

		dbtable_load_table_data(table, where); check(table_loadwhere_try001)

	catch(table_loadwhere_try001)
		on(Exception)
			if(table) TObject_Destroy(table, NULL);

			throw(ExceptionDBTableLoadingData, NULL)
		end_on
	end_try(table_loadwhere_try001)

    return table;
}

/*
 * DBTable_AddReg
 *
 * Historico:
 *
 * ...:
 *  Criada
 * 25-NOV-2018, Danilo:
 *  Terminada.
 * 26-NOV-2018, Danilo:
 *  Revisada. Algumas funções migraram para o módulo dbrow
 * 30-NOV-2018, Danilo:
 *  Revisada. A funcionalidade passou para a função auxiliar.
 */
TStatus   DBTable_AddRow   (TDBTable *table, TDBRow *row) {
    onerror( TString_CopyF((TString*)&row->cells->start->item, "%lu", table->availableId) )
		throw_note( ExceptionDBTableAddRow, FAIL, "Tentativa de atribuir o id disponivel" )

	onerror( OK==dbtable_addrow_aux(table, row, eINSERT) )
		throw( ExceptionDBTableAddRow, FAIL )

    table->availableId++;
    return OK;
}
/*
 * DBTable_DelRow
 *
 * Historico:
 *
 * ...:
 *  Criada
 * 02-dez-2018, Danilo:
 *  Terminada.
 */
TStatus   DBTable_DelRow   (TDBTable *table, TDBRow *row) {
    return dbtable_delrow_aux(table, row, FALSE);
}

static TStatus   dbtable_delrow_aux   (TDBTable *table, TDBRow *row, TBool ignoreVerif) {

    if(TRUE == ignoreVerif) {
        row->needChange = eDELETE;
        return OK;
    } else {
        LOOPLIST(table->rows,)
            if(_NODE_->item == (void *)row) {
                row->needChange = eDELETE;
                return OK;
            }
        END_LOOPLIST
    }

    throw( ExceptionDBTableNotFoundRow, FAIL )
}

TDBRow   *DBTable_GetRowById   (TDBTable *table, char *idAsStr) {
    LOOPLIST(table->rows,)
        if(!strcmp((char *)((TDBRow *)_NODE_->item)->cells->start->item, idAsStr))
            return (TDBRow *)_NODE_->item;
    END_LOOPLIST

    throw_note( ExceptionDBTableNotFoundRow, NULL, "O id fornecido não corresponde a nenhum registro" )
}

TDBRow   *DBTable_GetRowByFinder   (TDBTable *table, FRowFinder finder, void *finderData) {
    LOOPLIST(table->rows,)
        if(finder((TDBRow *)_NODE_->item, finderData))
            return (TDBRow *)_NODE_->item;
    END_LOOPLIST

    return NULL;
}

/*
 * DBTable_WriteChanges
 *
 * Historico:
 *
 * ...:
 *  Criada.
 * 04-DEZ-2018, Danilo:
 *  Revisada. Transferida de cursor para table. Documentada.
 * 06-DEZ-2018, Danilo:
 *  Terminada.
 */
TStatus      DBTable_WriteChanges(TDBTable *table) {
    TDBRow *row;
    TString sql = NULL;

    try(dbtwc_001)
		LOOPLIST(table->rows,)
			row = (TDBRow *)_NODE_->item;

			switch(row->needChange) {
			case eINSERT:
				sql = row_to_sql_insert(row, table); check(dbtwc_001)
				break;
			case eUPDATE:
				sql = row_to_sql_update(row, table); check(dbtwc_001)
				break;
			case eDELETE:
				sql = row_to_sql_delete(row, table); check(dbtwc_001)
				break;
			default:
				CONTINUE_LOOP_LIST
			}

			TSQLite3Conn_ExecSQL(table->conn, sql, NULL, NULL); check(dbtwc_001)

			sql = TObject_Destroy(sql, NULL);
		END_LOOPLIST

	catch(dbtwc_001)
		on(Exception)
			if(sql)
				TObject_Destroy(sql, NULL);
			throw(ExceptionDBTableWriteChanges, FAIL)
		end_on
	end_try(dbtwc_001)

    return OK;
}

