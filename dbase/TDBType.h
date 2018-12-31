/*
 * Type.h
 *
 *  Created on: 6 de dez de 2018
 *      Author: root
 */

#ifndef DBASE_TYPES_H_
#define DBASE_TYPES_H_

#include "../Type.h"
#include "../base/TList.h"
#include "../str/TString.h"
#include "../interfaces/dbase/IDBConn.h"
#include "../dbase/TDBField.h"

typedef struct DBTable      TDBTable;
typedef struct DBRow         TDBRow;

typedef struct DBCursor         TDBCursor;

typedef int (*FRowFinder)(TDBRow *row, void *data);

struct DBTable {
    TCount    rowsCount;
    TCount    fieldsCount;
    TId       availableId;
    TBool     isLoaded;
    TList    *rows;
    IDBConn  *conn;
    TList    *fields;
    TString   name;
};

enum DBRowChangeType {
    eNONE = 0,
    eINSERT,
    eUPDATE,
    eDELETE
};
typedef enum DBRowChangeType TDBRowChangeType;

struct DBRow {
    TList            *cells;
    TDBRowChangeType  needChange;
    TDBTable         *parentTable;
};

struct DBCursor {
    TDBTable  *table;
    TLstNod *rowCursor;
    TSize      pageSize;
};

enum DBCsrDirection {
    eNEXT,
    ePREV,
    eFIRST,
    eLAST,
    ePGDOWN,
    ePGUP
};
typedef enum   DBCsrDirection   TDBCsrDirection;


#endif /* DBASE_TYPES_H_ */
