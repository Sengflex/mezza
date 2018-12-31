/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   dbconnection.c
 * Author: root
 *
 * Created on 22 de Novembro de 2018, 15:16
 */
#define MZ_IDBCONN_IMPLEMENT
#include "IDBConn_sqlite3.h"

 #include <string.h>

#include "TDBTable.h"
#include "../Lang.h"
#include "../OnError.h"
#include "../base/TMemMgr.h"
#include "../base/TObject.h"
#include "../interfaces/dbase/IDBConn.h"

#include <sqlite3.h>

static TStatus sqlite3_connect_function(IDBConn *self, TString user, TString pass);
static TStatus sqlite3_disconnect_function(IDBConn *self);
static TStatus sqlite3_execSql_function(IDBConn *self, char *sql, FConnExec fexec, void *fexecData);


/*
 * IDBConn_CreateSQLite3
 *
 * Historico:
 *
 * 30-NOV-2018, Danilo:
 *  Criada. Terminada
 */
IDBConn *IDBConn_CreateSQLite3(TMemMgr *memmgr, char *filename) {
    IDBConn *ret = IDBConn_Create(
            memmgr,
            filename,
            0,
            sqlite3_connect_function,
            sqlite3_disconnect_function,
            sqlite3_execSql_function
            );

    onerror(ret)
        throw_note(ExceptionIDBConnCreation, NULL, "SQLite3")

    return ret;
}

/*
 * sqlite3_connect_function ('IDBConn.connect()')
 *
 * Historico:
 *
 * 30-NOV-2018, Danilo:
 *  Criada
 */
static TStatus sqlite3_connect_function(IDBConn *self, TString user, TString pass) {
    int rc;

    rc = sqlite3_open(self->hostOrFile, (sqlite3 **)&self->connection);
    onerror(rc==0) {
        char note[500];
        sprintf(note, "SQLite3 implementation: %s", sqlite3_errmsg((sqlite3 *)self->connection));

        sqlite3_close((sqlite3 *)self->connection);

        throw_note(ExceptionIDBConnActivation, FAIL, note)
    }

    self->isConnected = TRUE;

    return OK;
}

/*
 * sqlite3_disconnect_function ('IDBConn.disconnect()')
 *
 * Historico:
 *
 * 30-NOV-2018, Danilo:
 *  Criada
 */
static TStatus sqlite3_disconnect_function(IDBConn *self) {
    sqlite3_close((sqlite3 *)self->connection);

    return OK;
}

/*
 * sqlite3_execSql_function ('IDBConn.execSql()')
 *
 * Throws: ExceptionIDBConnExecution
 *
 * Historico:
 *
 * ...:
 *  Criada
 * 25-NOV-2018, Danilo:
 *  Terminada.
 * 29-NOV-2018, Danilo:
 *  Revisada.
 * 30-NOV-2018, Danilo:
 *  Atualizada para a interface IDBConn.
 */
static TStatus sqlite3_execSql_function(IDBConn *self, char *sql, FConnExec fexec, void *fexecData) {
    char *zErrMsg = NULL;
    int rc;

    onerror( self->isConnected == TRUE )
        throw(ExceptionIDBConnActivation, FAIL)

   /* Execulta a consulta e pôe os dados numa tabela */
    rc = sqlite3_exec((sqlite3 *)self->connection, sql, fexec, fexecData, &zErrMsg);
    onerror( rc  == SQLITE_OK ) {
        char note[500];
        sprintf(note, "SQLite3 implementation: %s", zErrMsg);
        sqlite3_free(zErrMsg);
        throw_note(ExceptionIDBConnExecution, FAIL, note)
    }

    return OK;
}
