/*
 * TDBField.h
 *
 *  Created on: 14 de dez de 2018
 *      Author: root
 */
#ifndef DBASE_FIELD_H_
#define DBASE_FIELD_H_

#include "../Type.h"
#include "../base/TMemMgr.h"
#include "../base/Exceptions.h"

#ifndef MZ_DBFIELD_IMPLEMENT
    declare_exception(ExceptionDBFIeldCreation)
    declare_exception(ExceptionDBFIeldVectorCreation)
#else
    define_exception(ExceptionDBFIeldCreation,      "Falha em criar o objeto TDBField")
    define_exception(ExceptionDBFIeldVectorCreation,      "Falha em criar o vetor de objetos TDBField")
#endif


typedef struct DBField TDBField;

#define PRIMARY_KEY  1
#define UNIQUE       2
#define NOT_NULL     4
#define AUTO_INC     8
#define DEFAULT_VAL 16

enum   DBFieldType {
    eCHAR,
    eTEXT,
    eSHORT,
    eINT,
    eLONG,
    eFLOAT,
    eDOUBLE,
    eBUFFER
};
typedef enum   DBFieldType TDBFieldType;

struct DBField {
    char        *name;
    TDBFieldType type;
    unsigned int contraints;
    char        *defaultVal;
};
/**
 *
 * Dispara: ExceptionDBFIeldCreation
 * */
TDBField *DBField_Create(TMemMgr *memmgr, char *name, TDBFieldType type, unsigned int contraints, char *defaultVal);
/**
 * NOTE: pode ser desnecessario. Checar quando o módulo estiver completo
 * Dispara: ExceptionDBFIeldVectorCreation
 * */
TDBField *DBField_CreateVector(TMemMgr *memmgr, TDBField *staticFieldVec, TCount staticFieldVecSz);

#endif /* DBASE_FIELD_H_ */
