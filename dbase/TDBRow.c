/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   dbrow.c
 * Author: root
 *
 * Created on 22 de Novembro de 2018, 15:27
 */
#define MZ_DBROW_IMPLEMENT
#include "TDBRow.h"

#include "../OnError.h"
#include "../Lang.h"
#include "../Type.h"
#include "../base/TList.h"
#include "../base/TObject.h"
#include "../str/TString.h"
#include "../str/TStringVector.h"

#include <stdio.h>
#include <stdarg.h>

static void row_objdtor(TObject addr, void *ignore);
static void _aux_restore_state(va_list ap, int i);

/*
 * DBTable_AddReg
 *
 * Historico:
 *
 *  Criada
 * 25-NOV-2018, Danilo:
 *  Terminada.
 * 30-NOV-2018, Danilo:
 *  Revisada. Lógica e relatório de erro
 */

TDBRow *TDBRow_Create(TMemMgr *memmgr, char **cells, TSize cellsSz) {
    TDBRow *ret = NULL;
    /* Vetor temporario para as celulas. Visa facilitar reversao em caso de
     * erro. Obs.: Quando uma lista e desalocada, os itens (neste caso as
     * celulas ja alocadas) permanecem intocados e devem ser desalocados em
     * separado. Estando num vetor, eles serao desalocados junto com o vetor */
    TString   *vstrTmp=NULL;

    try(dbrwc_001)

		/* Alocação de objeto */
		ret = TObject_CreateZero(memmgr, sizeof(TDBRow), row_objdtor); check(dbrwc_001)

		/* Criação da lista de células */
		ret->cells = TList_Create(memmgr); check_note(dbrwc_001, "Inicializacao. Campo 'cells'")

		/* Criação de vetor temporário */
		vstrTmp = TStringVector_Create(TObject_ManagerOf(ret), cells, cellsSz); check_note(dbrwc_001, "Vetor temporario")

		/* Preenchimento da lista de células a partir do vetor temporário */
		loop_i(cellsSz)
			TList_Add(ret->cells, vstrTmp[_INDEX_]); check_note(dbrwc_001, "Inicializacao. Preenchimento da lista 'cells'")
		end_loop_i

		/* Deleta o vetor sem cesalocar as celulas (agora elas estao no
			 * registro). Esta desalocaçao deve estar apos confirmado que as
			 * celulas nao precisarao ser eliminadas como medida de reversao em
			 * caso de erro. Ou seja: NAO TIRE ISSO DAQUI! */
		TObject_DestroyByPassDtor(vstrTmp, NULL);

		/* Terminando a inicialização do objeto */
		ret->needChange = eNONE;
		ret->parentTable = NULL;

	catch(dbrwc_001)
		on(Exception)
			if(vstrTmp) TObject_Destroy(vstrTmp, NULL);
			if(ret) TObject_Destroy(ret, NULL);
			throw_note(ExceptionDBRowCreation, NULL, _TRY_NOTE_)
		end_on
	end_try(dbrwc_001)

    return ret;
}

/*
 * TDBRow_Get
 *
 * Historico:
 *
 * ...:
 *  Criada.
 * 26-NOV-2018, Danilo:
 *  Terminada.
 * 04-DEZ-2018, Danilo:
 *  Revisada. Documentada.
 * 06-DEZ-2018, Danilo:
 *  Migrou de cursor para row.
 */
TStatus      TDBRow_Get(TMemMgr *memmgr, TDBRow *row, TCount nofParams, ...) {
    va_list ap;
    TCount nofIterations = nofParams;
    TString *cell;
    TNode *node = row->cells->start;

    if(row->parentTable) {
        if(row->parentTable->fieldsCount < nofIterations)
            nofIterations = row->parentTable->fieldsCount;
    }

    va_start ( ap, nofParams );
    loop_i(nofIterations)
        cell = va_arg ( ap, TString* );

        onerror( (*cell = TString_Create(memmgr, (TString)node->item, 0)) ) {
        	va_end(ap);
			va_start(ap, nofParams);
			_aux_restore_state(ap, _INDEX_);
			va_end(ap);
        	throw_note(ExceptionDBRowGet, FAIL, "Alocacao de objeto TString para conteiner de celula")
        }
        node = node->next;
    end_loop_i
    va_end(ap);

    return OK;
}

/*
 * TDBRow_Set
 *
 * Historico:
 *
 * ...:
 *  Criada.
 * 26-NOV-2018, Danilo:
 *  Terminada.
 * 04-DEZ-2018, Danilo:
 *  Revisada. Documentada.
 * 06-DEZ-2018, Danilo:
 *  Migrou de cursor para row.
 */
TStatus      TDBRow_Set(TMemMgr *memmgr, TDBRow *row, TCount nofParams, ...) {
    va_list   ap;
    TCount    nofIterations = nofParams;
    TString   cell;
    TString   *vstrTmp;
    TString   *vstrTmpCursor;

    /* Calcula o numero de iteraçoes correto.
     * Se houver diferença entre 'nofParams' e o numero de campos da tabela,
     * esta medida evitara erros. O registro pode, no entanto ser atribuido
     * parcialmente. */
    if(row->parentTable) {
        if(row->parentTable->fieldsCount < nofIterations)
            nofIterations = row->parentTable->fieldsCount;
    } else {
        if(row->cells->size < nofIterations)
            nofIterations = row->cells->size;
    }

    /* Aloca vetor temporario de strings */
    onerror( (vstrTmp = TStringVector_Create(memmgr, NULL, nofIterations)) )
    	throw_note(ExceptionDBRowSet, FAIL, "Criacao de vetor temporario")

    /* Preenche o vetor temporario */
    va_start ( ap, nofParams );
    loop_i(nofIterations)
        cell = va_arg ( ap, TString );

        onerror( (vstrTmp[_INDEX_] = TString_Create(memmgr, cell, 0)) ) {
        	va_end(ap);
			TObject_Destroy(vstrTmp, NULL);
        	throw_note(ExceptionDBRowSet, FAIL, "Atribuicao de vetor temporario")
        }
    end_loop_i
    va_end(ap);

    /* Transfere strings do vetor temporario para a lista do registro corrente */
    vstrTmpCursor = vstrTmp;
    LOOPLIST(row->cells, )
        TObject_Destroy(_NODE_->item, NULL);
        _NODE_->item = *vstrTmpCursor;
        *vstrTmpCursor = NULL;
        vstrTmpCursor++;
    END_LOOPLIST

    /* Libera o vetor temporario */
    TObject_Destroy(vstrTmp, NULL);

    /* Sinaliza que precisa atualizar a base de dados de origem */
    row->needChange = eUPDATE;

    return OK;
}

TString row_to_sql_update(TDBRow *row, TDBTable *table) {
    TString      sql=NULL;
    TNode *nodeFields;
    TNode *nodeCells;

    try(rwtsqlu_001)

		sql = TString_Create(TObject_ManagerOf(row), NULL, 300); check_note(rwtsqlu_001, "SQL UPDATE. Criacao de objeto TString para conteiner")

		TString_ConcatF(&sql, "UPDATE %s SET", table->name); check_note(rwtsqlu_001, "SQL UPDATE. Adição de nome de tabela")

		nodeFields = table->fields->start->next;
		 nodeCells = row->cells->start->next;
		 while(nodeFields) {
			 TString_ConcatF(&sql, " %s = %s", (char *)((TDBField *)nodeFields->item)->name, (char *)nodeCells->item);
			 check_note(rwtsqlu_001, "SQL UPDATE. Adição de campos")

			 if(nodeFields->next) {
				 TString_Concat(&sql, ","); check_note(rwtsqlu_001, "SQL UPDATE. Adição de campos")
			 }
			 nodeCells = nodeCells->next;
			 nodeFields = nodeFields->next;
		 }

		 TString_ConcatF(&sql, " WHERE id = %s", (char *)row->cells->start->item);
		 check_note(rwtsqlu_001, "SQL UPDATE. Adição do parâmetro WHERE")

	catch(rwtsqlu_001)
		on(Exception)
			if(sql) TObject_Destroy(sql, NULL);
		 	throw_note(ExceptionDBRowSQLGen, NULL, _TRY_NOTE_)
		end_on
	end_try(rwtsqlu_001)

    return sql;
}

TString row_to_sql_delete(TDBRow *row, TDBTable *table) {
    TString      sql;

    onerror( (sql = TString_Create(TObject_ManagerOf(row), NULL, 300)) )
    	throw_note(ExceptionDBRowSQLGen, NULL, "SQL DELETE. Criacao de objeto TString para conteiner")

	onerror( TString_ConcatF(&sql, "DELETE FROM %s WHERE id=%s", table->name, (char *)row->cells->start->item) ) {
    	TObject_Destroy(sql, NULL);
    	throw_note(ExceptionDBRowSQLGen, NULL, "SQL DELETE. Adição de nome de tabela")
    }

    return sql;
}

TString row_to_sql_insert(TDBRow *row, TDBTable *table) {
    TString      sql;
    TNode *node;
    char aspasSimples = 0x27;

    try(rwtsqli_001)

		sql = TString_Create(TObject_ManagerOf(row), NULL, 300); check_note(rwtsqli_001, "SQL INSERT. Criacao de objeto TString para conteiner")

		TString_ConcatF(&sql, "INSERT INTO %s (", table->name); check_note(rwtsqli_001, "SQL INSERT. Adição de nome de tabela")

		LOOPLIST(table->fields,)
			TString_ConcatF(&sql, " %s", ((TDBField*)_NODE_->item)->name); check_note(rwtsqli_001, "SQL INSERT. Adição de campos")

			if(_NODE_->next) {
				TString_Concat(&sql, ","); check_note(rwtsqli_001, "SQL INSERT. Adição de campos")
			}
		END_LOOPLIST

		TString_Concat(&sql, ") VALUES ("); check_note(rwtsqli_001, "SQL INSERT. Adição do parâmetro VALUES")

		node = row->cells->start;
		LOOPLIST(table->fields,)
			switch(((TDBField *)_NODE_->item)->type) {
				case eCHAR:
				case eTEXT:
					TString_ConcatF(&sql, "%c%s%c", aspasSimples, (char *)node->item, aspasSimples);
					check_note(rwtsqli_001, "SQL INSERT. Adição do parâmetro VALUES")
					break;
				default:
					TString_ConcatF(&sql, "%s", (char *)node->item);
					check_note(rwtsqli_001, "SQL INSERT. Adição de valores para os campos")
					break;
			}
			if(_NODE_->next) {
				TString_Concat(&sql, ","); check_note(rwtsqli_001, "SQL INSERT. Adição de campos")
			}
			node = node->next;
		END_LOOPLIST

		TString_Concat(&sql, ")"); check_note(rwtsqli_001, "SQL INSERT. Fechamento do comando")
	catch(rwtsqli_001)
		on(Exception)
			if(sql)
				TObject_Destroy(sql, NULL);
			throw_note(ExceptionDBRowSQLGen, NULL, _TRY_NOTE_)
		end_on
	end_try(rwtsqli_001)

    return sql;
}

static void row_objdtor(TObject obj, void *ignore) {
    TDBRow *row = ((TDBRow *)obj);

    if(row->cells) {
        LOOPLIST(row->cells, TObject_Destroy(_NODE_->item, NULL);) END_LOOPLIST
        TObject_Destroy(row->cells, NULL);
    }
}

static void _aux_restore_state(va_list ap, int nofDestructions) { /* Destroi os objetos criados (strings) */
    TString *cell;

    loop_i(nofDestructions)
        cell  = va_arg ( ap, TString* );
        *cell = TObject_Destroy(*cell, NULL);
    end_loop_i
}
