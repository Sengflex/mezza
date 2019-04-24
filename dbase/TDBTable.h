/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   dbTDBTable.h
 * Author: root
 *
 * Created on 22 de Novembro de 2018, 15:22
 */

#ifndef MZ_DBASE_DBTABLE_H
#define MZ_DBASE_DBTABLE_H

#include "../Type.h"
#include "../str/TString.h"
#include "TSQLite3Conn.h"
#include "TDBType.h"
#include "../base/Exceptions.h"

#ifndef MZ_DBASE_DBTABLE_IMPLEMENT
    declare_exception(ExceptionTableCreation)
    declare_exception(ExceptionDBTableAllocation)
    declare_exception(ExceptionTableInternals)
    declare_exception(ExceptionDBTableLoadingData)
    declare_exception(ExceptionDBTableAddRow)
    declare_exception(ExceptionDBTableNotFoundRow)
    declare_exception(ExceptionDBTableRowInsertion)
    declare_exception(ExceptionDBTableRowUpdate)
    declare_exception(ExceptionDBTableRowDelete)
    declare_exception(ExceptionDBTableWriteChanges)
#else
    define_exception(ExceptionTableCreation,      "Falha em criar o objeto tabela")
    define_exception(ExceptionDBTableAllocation,  "Falha em alocar o objeto tabela")
    define_exception(ExceptionTableInternals,     "Falha em operacao interna da tabela")
    define_exception(ExceptionDBTableLoadingData, "Falha na leitura dos dados da tabela")
    define_exception(ExceptionDBTableAddRow,      "Falha em adicionar registro")
    define_exception(ExceptionDBTableNotFoundRow, "Registro nao encontrado")
    define_exception(ExceptionDBTableRowInsertion, "Falha em inserir registro")
    define_exception(ExceptionDBTableRowUpdate, "Falha em atualizar registro")
    define_exception(ExceptionDBTableRowDelete, "Falha em deletar registro")
    define_exception(ExceptionDBTableWriteChanges, "Falha em escrever as mudanças")
#endif

/**
 * Cria uma tabela de dados
 *
 * A tabela é um objeto que conterá os dados de uma consulta a
 * um banco de dados (sqlite3).
 *
 * @param conn        Conexão à base de dados
 * @param name        Nome da tabela na base de dados
 * @param vfields      Vetor de campos
 * @param vfieldsCount Tamanho do vetor 'vfields'
 *
 * @return Um objeto TDBTable. NULL em caso de erro.
 *
 * Dispara: ExceptionTableCreation
 * */
TDBTable *TDBTable_Create   (TMemMgr *memmgr, TSQLite3Conn *conn, char *name, TDBField *vfields, TCount vfieldsCount);
/**
 * Realiza a consulta no banco de dados e carrega os resultados na tabela
 *
 * @param table Tabela criada com DBTable_Create
 * @return OK para sucesso. FAIL para falha
 * */
TDBTable *TDBTable_Load(TMemMgr *memmgr, TSQLite3Conn *conn, char *name);
/**
 * Realiza a consulta no banco de dados e carrega os resultados na tabela
 *
 * @param table Tabela criada com DBTable_Create
 * @param where Condição para o comando WHERE. Permite consulta seletiva
 * @return OK para sucesso. FAIL para falha
 * */
TDBTable   *TDBTable_LoadWhere(TMemMgr *memmgr, TSQLite3Conn *conn, char *name, char *where);
/**
 * Adiciona um objeto registro/linha a uma tabela
 *
 * @param table Tabela criada com DBTable_Create
 * @param row Objeto registro/linha a ser adicionado
 *
 * Dispara: ExceptionDBTableAddRow
 * */
TStatus   DBTable_AddRow   (TDBTable *table, TDBRow *row);
/**
 * Deleta um registro da tabela fazendo toda a limpeza de memória
 * necessária
 *
 * @param table Tabela criada com DBTable_Create
 * @param row Registro existente na tabela
 *
 * @return OK em caso de sucesso. FAIL em caso de falha
 *
 * Dispara: ExceptionDBTableNotFoundRow
 * */
TStatus   DBTable_DelRow   (TDBTable *table, TDBRow *row);
/**
 * Obtem um registro da tabela pesquisando pelo id
 *
 * @param table Tabela criada com DBTable_Create
 * @param idAsStr Id a ser procurado
 *
 * @return O objeto registro correspondente. NULL em caso de falha
 *
 * Dispara: ExceptionDBTableNotFoundRow
 * */
TDBRow   *DBTable_GetRowById   (TDBTable *table, char *idAsStr);
/**
 * Obtem um registro da tabela pesquisando por dados quaisquer
 *
 * @param table Tabela criada com DBTable_Create
 * @param finder Callback que identifica um registro baseado nos dados de comparação
 * @param finderData Dados de comparação a serem passados para o 'finder'
 *
 * @return O objeto registro correspondente. NULL em caso de falha
 *
 * */
TDBRow   *DBTable_GetRowByFinder   (TDBTable *table, FRowFinder finder, void *finderData);
/**
 * Escreve as mudanças da tabela para a base de dados
 *
 * @param table Tabela criada com DBTable_Create
 *
 * @return OK em caso de sucesso. FAIL em caso de falha
 *
 * Dispara: ExceptionDBTableRowInsertion
 * Dispara: ExceptionDBTableRowUpdate
 * Dispara: ExceptionDBTableRowDelete
 * */
TStatus   DBTable_WriteChanges(TDBTable *table);

#endif /* MZ_DBASE_DBTABLE_H */
