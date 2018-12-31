/*
 * File:   IDBConn_sqlite3.h
 * Author: Danilo Almeida
 *
 * Created on 30 de Novembro de 2018, 15:16
 *
 * Implementa a interface TDBase em dbase/IDBConn.h
 */

#ifndef MZ_DBASE_DBCONNECTION_H
#define MZ_DBASE_DBCONNECTION_H

#include "../interfaces/dbase/IDBConn.h"
#include "../base/Exceptions.h"

/*
 * Cria um objeto que implementa IDBConn
 *
 * Obs,: Como parte da interface o destrutor chamará automaticamente
 * o método IDBConn.disconnect, que deverá ser implementado para
 * encerrar a conexão
 *
 * @param memmgr Gerenciador de alocações de memória
 * @param filename Nome do arquivo onde está a base de dados sqlite3
 *
 * @return Um objeto IDBConn com atributos e métodos da interface
 * inicializados. NULL em caso de erro
 *
 * Throws: ExceptionIDBConnSQLite3Creation
 */
IDBConn     *IDBConn_CreateSQLite3  (TMemMgr *memmgr, char *filename);

#endif /* MZ_DBASE_DBCONNECTION_H */
