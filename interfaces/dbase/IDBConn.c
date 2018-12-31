/*
 * connection.c
 *
 *  Created on: 30 de nov de 2018
 *      Author: root
 */
#define MZ_INTERFACES_DBASE_CONN_IMPLEMENT
#include "IDBConn.h"

#include "../../OnError.h"

#include "../../base/TMemMgr.h"
#include "../../base/TObject.h"

static void idbconn_dtor(TObject obj, void *ignore);



/*
 * DBTable_AddReg
 *
 * Historico:
 *
 * ...:
 *  Criada
 * 01-DEZ-2018, Danilo:
 *  Terminada.
 */
IDBConn *IDBConn_Create(
                TMemMgr      *memmgr,
                TString       hostOrFile,
                unsigned int  port,
                TStatus     (*connect_function)(IDBConn *self, TString user, TString pass),
                TStatus     (*disconnect_function)(IDBConn *self),
                TStatus     (*execSql_function)(IDBConn *self, char *sql, FConnExec fexec, void *fexecData)
        ) {
    IDBConn *ret;

    ret = TObject_Create(memmgr, sizeof(IDBConn), idbconn_dtor);
    onerror( ret )
		throw(ExceptionIDBConnCreation, NULL)

    ret->hostOrFile = TString_Create(memmgr, hostOrFile, 0);
    onerror( ret->hostOrFile ) {
    	TObject_DestroyByPassDtor(ret, NULL);
		throw(ExceptionIDBConnCreation, NULL)
    }

    ret->port         = port;
    ret->connection   = NULL;
    ret->isConnected = FALSE;

    ret->connect      = connect_function;
    ret->disconnect   = disconnect_function;
    ret->execSql      = execSql_function;

    return ret;
}

static void idbconn_dtor(TObject obj, void *ignore) {
    IDBConn       *conn           = (IDBConn *)obj;

    TObject_Destroy(conn->hostOrFile, NULL);

    if(conn->disconnect)
        conn->disconnect(conn);
}
