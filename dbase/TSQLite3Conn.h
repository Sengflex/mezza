
/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * dbase/TSQLite3Conn_sqlite3.h
 *
 *  Created on: 30 de Novembro de 2018, 15:16
 *      Author: Danilo Almeida
 */

#ifndef MZ_DBASE_DBCONNECTION_H
#define MZ_DBASE_DBCONNECTION_H

#include <sqlite3.h>
#include "../base/Exceptions.h"
#include "../str/TString.h"
#include "../Type.h"

#ifndef DBASE_SQLITE3CONN_IMPLEMENT
    declare_exception(ExceptionTSQLite3ConnCreation)
    declare_exception(ExceptionTSQLite3ConnActivation)
    declare_exception(ExceptionTSQLite3ConnExecution)
#else
    define_exception(ExceptionTSQLite3ConnCreation, "Falha em criar objeto TSQLite3Conn")
	define_exception(ExceptionTSQLite3ConnActivation, "A conexao nao foi ativada")
	define_exception(ExceptionTSQLite3ConnExecution, "Comando SQL falhou")
#endif
typedef int  SQLite3ExecCallback(void *data, int argc, char **argv, char **azColName);

typedef struct SQLite3Conn TSQLite3Conn;

struct SQLite3Conn {
    TString  fileName;
    sqlite3 *connection;
    TBool    isConnected;
};

/*
 * Cria um objeto TSQLite3Conn
 *
 * Obs.: O destrutor encerrará automaticamente a conexão
 *
 * @param memmgr    Gerenciador de alocações de memória
 * @param filename  Nome do arquivo em que está a base de dados sqlite3
 *
 * @return Um novo objeto TSQLite3Conn. NULL em caso de erro
 *
 * Dispara: ExceptionTSQLite3ConnCreation
 */
TSQLite3Conn *TSQLite3Conn_Create  (TMemMgr *memmgr, char *filename);
/**
 * Ativa a conexão
 * 
 * Prepara o objeto TSQLite3Conn para as demais operações
 * 
 * @param conn Objeto criado com TSQLite3Conn_Create
 * 
 * Dispara: ExceptionTSQLite3ConnActivation
 */
TStatus TSQLite3Conn_Connect(TSQLite3Conn *conn);
/**
 * Destiva a conexão
 * 
 * Operações seguintes irão falhar após a desativação
 * 
 * @param conn Objeto criado com TSQLite3Conn_Create
 */
TStatus TSQLite3Conn_Disconnect(TSQLite3Conn *conn);
/**
 * Executa um comando SQL através da conexão
 * 
 * O comando é executado e uma função de callback será usada para 
 * manipular o resultado
 * 
 * @param conn Objeto criado com TSQLite3Conn_Create
 * @param sql Comando SQL
 * @param fexec Callback de manipulação de resultado
 * @param fexecData Dados passados ao callback
 * 
 * Dispara: ExceptionTSQLite3ConnActivation, ExceptionTSQLite3ConnExecution
 */
TStatus TSQLite3Conn_ExecSQL(TSQLite3Conn *conn, char *sql, SQLite3ExecCallback fexec, void *fexecData);

#endif /* MZ_DBASE_DBCONNECTION_H */
