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
#define DBASE_SQLITE3CONN_IMPLEMENT
#include "TSQLite3Conn.h"

#include <string.h>
#include "TDBTable.h"
#include "../Lang.h"
#include "../OnError.h"
#include "../base/TMemMgr.h"
#include "../base/TObject.h"

#include <sqlite3.h>

static void TSQLite3Conn_dtor(TObject obj, void *ignore);


/*
 * TSQLite3Conn_Create
 *
 * Historico:
 *
 * 30-NOV-2018, Danilo:
 *  Criada. Terminada
 * 24-ABR-2019, Danilo, versão 0.2.0: 
 *      [INCOMPATIBILIDADE com 0.1.0]
 *      Rótulo modificado. 
 *      Interface abandonada em favor de struct.
 */
TSQLite3Conn *TSQLite3Conn_Create(TMemMgr *memmgr, char *fileName) {
    TSQLite3Conn *ret;

    #ifdef DEBUG
        assert(memmgr);
        assert(fileName);
    #endif

    ret = TObject_Create(memmgr, sizeof(TSQLite3Conn), TSQLite3Conn_dtor);
    onerror( ret )
		throw(ExceptionTSQLite3ConnCreation, NULL)

    ret->fileName = TString_Create(memmgr, fileName, 0);
    onerror( ret->fileName ) {
    	TObject_DestroyByPassDtor(ret, NULL);
		throw(ExceptionTSQLite3ConnCreation, NULL)
    }
    ret->connection   = NULL;
    ret->isConnected = FALSE;

    return ret;
}

/*
 * TSQLite3Conn_Connect ('TSQLite3Conn.connect()')
 *
 * Historico:
 *
 * 30-NOV-2018, Danilo:
 *  Criada
 * 24-ABR-2019, Danilo, versão 0.2.0: 
 *      [INCOMPATIBILIDADE com 0.1.0]
 *      Rótulo modificado. 
 *      Interface abandonada em favor de struct.
 */
TStatus TSQLite3Conn_Connect(TSQLite3Conn *conn) {
    int rc;

    #ifdef DEBUG
        assert(conn);
    #endif

    if(!conn->isConnected) {
        rc = sqlite3_open(conn->fileName, (sqlite3 **)&conn->connection);
        onerror(rc==0) {
            char note[500];
            sprintf(note, "SQLite3 implementation: %s", sqlite3_errmsg((sqlite3 *)conn->connection));
            sqlite3_close((sqlite3 *)conn->connection);
            throw_note(ExceptionTSQLite3ConnActivation, FAIL, note)
        }

        conn->isConnected = TRUE;
    }

    return OK;
}

/*
 * TSQLite3Conn_Disconnect 
 *
 * Historico:
 *
 * 30-NOV-2018, Danilo:
 *  Criada
 * 24-ABR-2019, Danilo, versão 0.2.0: 
 *      [INCOMPATIBILIDADE com 0.1.0]
 *      Rótulo modificado. 
 *      Interface abandonada em favor de struct.
 */
TStatus TSQLite3Conn_Disconnect(TSQLite3Conn *conn) {
    #ifdef DEBUG
        assert(conn);
    #endif

    if(conn->isConnected) {
        sqlite3_close((sqlite3 *)conn->connection);
        conn->isConnected = FALSE;
    }

    return OK;
}

/*
 * TSQLite3Conn_ExecSQL ('TSQLite3Conn.execSql()')
 *
 * Dispara: ExceptionTSQLite3ConnExecution
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
 *  Atualizada para a interface TSQLite3Conn.
 * 24-ABR-2019, Danilo, versão 0.2.0: 
 *      [INCOMPATIBILIDADE com 0.1.0]
 *      Rótulo modificado. 
 *      Interface abandonada em favor de struct.
 */
TStatus TSQLite3Conn_ExecSQL(TSQLite3Conn *conn, char *sql, SQLite3ExecCallback fexec, void *fexecData) {
    char *zErrMsg = NULL;
    int rc;

    #ifdef DEBUG
        assert(conn);
        assert(sql);
        assert(fexec);
    #endif

    onerror( conn->isConnected )
        throw(ExceptionTSQLite3ConnActivation, FAIL)

   /* Execulta a consulta e pôe os dados numa tabela */
    rc = sqlite3_exec((sqlite3 *)conn->connection, sql, fexec, fexecData, &zErrMsg);
    onerror( rc  == SQLITE_OK ) {
        char note[500];
        sprintf(note, "SQLite3 implementation: %s", zErrMsg);
        sqlite3_free(zErrMsg);
        throw_note(ExceptionTSQLite3ConnExecution, FAIL, note)
    }

    return OK;
}

static void TSQLite3Conn_dtor(TObject obj, void *ignore) {
    TSQLite3Conn *conn = (TSQLite3Conn *)obj;

    /* Sempre que o objeto for destruído, a conexão será fechada 
    através da biblioteca sqlite3 */
    TSQLite3Conn_Disconnect(conn);

    if(conn->fileName)
        TObject_Destroy(conn->fileName, NULL);
}
