/*
 * IDBConn.h
 *
 *  Created on: 30 de nov de 2018
 *      Author: root
 */

#ifndef INTERFACES_DBASE_CONNECTION_H_
#define INTERFACES_DBASE_CONNECTION_H_

#include "../../str/TString.h"
#include "../../Type.h"
#include "../../base/TMemMgr.h"

#include "../../base/Exceptions.h"

#ifndef MZ_INTERFACES_DBASE_CONN_IMPLEMENT
    declare_exception(ExceptionIDBConnCreation)
    declare_exception(ExceptionIDBConnActivation)
    declare_exception(ExceptionIDBConnExecution)
#else
    define_exception(ExceptionIDBConnCreation, "Falha em criar objeto IDBConn")
	define_exception(ExceptionIDBConnActivation, "A conexao nao foi ativada")
	define_exception(ExceptionIDBConnExecution, "Comando SQL falhou")
#endif

typedef int  FConnExec(void *data, int argc, char **argv, char **azColName);

typedef struct DBConn IDBConn;

struct DBConn {
    TString       hostOrFile;
    unsigned int  port;
    void         *connection;
    TBool         isConnected;

    TStatus (*connect)(IDBConn *self, TString user, TString pass);
    TStatus (*disconnect)(IDBConn *self);
    TStatus (*execSql)(IDBConn *self, char *sql, FConnExec fexec, void *fexecData);
};

/**
 * Cria um objeto IDBConn. Backend para implementações que disponibiliza
 * alocação e inicialização do objeto.
 *
 * @param memmgr Gerenciador de alocações
 * @param hostOrFile Nome do arquivo ou servidor
 * @param port Porta do servidor
 * @param connect_function Callback para conectar ao banco de dados
 * @param disconnect_function Callback para desconectar do banco de dados
 * @param execSql_function Callback para executar comandos SQL no banco de dados
 *
 * @return Um objeto IDBConn. NULL em caso de erro
 * */
IDBConn *IDBConn_Create(
                TMemMgr      *memmgr,
                TString       hostOrFile,
                unsigned int  port,
                TStatus     (*connect_function)(IDBConn *self, TString user, TString pass),
                TStatus     (*disconnect_function)(IDBConn *self),
                TStatus     (*execSql_function)(IDBConn *self, char *sql, FConnExec fexec, void *fexecData)
        );

#endif /* INTERFACES_DBASE_CONNECTION_H_ */
