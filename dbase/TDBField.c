
/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * dbase/TDBRow.h
 *
 *  Created on:  14 de dez de 2018
 *      Author: Danilo Almeida
 */

#define MZ_DBFIELD_IMPLEMENT
#include "TDBField.h"

#include <stdio.h>
#include <string.h>
#include "../Lang.h"
#include "../OnError.h"
#include "../base/TObject.h"
#include "../base/TMemMgr.h"
#include "../str/TString.h"

static void dbvfield_cleaner(TObject obj, void *ignore);

static void field_cleaner(TObject obj, void *ignore);

TDBFieldType str2fieldType(char *str);

TDBField *DBField_Create(TMemMgr *memmgr, char *name, TDBFieldType type, unsigned int constraints, char *defaultVal) {
    TDBField *field_ret=NULL;

    try(dbfdc_001)

		field_ret = TObject_CreateZero(memmgr, sizeof(TDBField), field_cleaner); check(dbfdc_001)

		field_ret->name = TString_Create(memmgr, name, 0); check_note(dbfdc_001, "Inicializacao. Campo 'name'")

		if(DEFAULT_VAL & constraints)
			field_ret->defaultVal = TString_Create(memmgr, defaultVal, 0); check_note(dbfdc_001, "Inicializacao. Campo 'defaultVal'")

		field_ret->type       = type;
		field_ret->contraints = constraints;

	catch(dbfdc_001)
		on(Exception)
			if(field_ret) {
				if(field_ret->name)
					TObject_Destroy(field_ret->name, NULL);
				TObject_DestroyByPassDtor(field_ret, NULL);
			}
			throw_note(ExceptionDBFIeldCreation, NULL, _TRY_NOTE_)
		end_on
	end_try(dbfdc_001)

    return field_ret;
}

TDBField *DBField_CreateVector(TMemMgr *memmgr, TDBField *staticFieldVec, TCount staticFieldVecSz) {
    TDBField *vfield_ret=NULL;

    try(dbfdcv_001)

		vfield_ret = TObject_Create(memmgr, sizeof(TDBField) * staticFieldVecSz, dbvfield_cleaner); check(dbfdcv_001)

		loop_i(staticFieldVecSz)
			vfield_ret[_INDEX_].name = NULL;
			vfield_ret[_INDEX_].defaultVal = NULL;
		end_loop_i

		if(staticFieldVec) {
			loop_i(staticFieldVecSz)
				vfield_ret[_INDEX_].name = TString_Create(memmgr, staticFieldVec[_INDEX_].name, 0); check_note(dbfdcv_001, "Inicializacao. Campo 'name'")

				if(vfield_ret[_INDEX_].contraints & DEFAULT_VAL)
					vfield_ret[_INDEX_].defaultVal = TString_Create(memmgr, staticFieldVec[_INDEX_].defaultVal, 0); check_note(dbfdcv_001, "Inicializacao. Campo 'defaultVal'")

				vfield_ret[_INDEX_].type = staticFieldVec[_INDEX_].type;
				vfield_ret[_INDEX_].contraints = staticFieldVec[_INDEX_].contraints;
			end_loop_i
		}

	catch(dbfdcv_001)
		on(Exception)
			if(vfield_ret)
				TObject_Destroy(vfield_ret, NULL);
			throw_note(ExceptionDBFIeldVectorCreation, NULL, _TRY_NOTE_)
		end_on
	end_try(dbfdcv_001)

    return vfield_ret;
}

static void dbvfield_cleaner(TObject obj, void *ignore) {
    TDBField *fieldVec = (TDBField *)obj;
    TSize fieldVecSz = TObject_SizeOf(obj) / sizeof(TDBField);

    loop_i(fieldVecSz)
        if(fieldVec[_INDEX_].name)
            TObject_Destroy(fieldVec[_INDEX_].name, NULL);
        if(fieldVec[_INDEX_].defaultVal)
            TObject_Destroy(fieldVec[_INDEX_].defaultVal, NULL);
    end_loop_i
}

static void field_cleaner(TObject obj, void *ignore) {
    TDBField *field = (TDBField *)obj;

    if(field->name)
        TObject_Destroy(field->name, NULL);
    if(field->defaultVal)
        TObject_Destroy(field->defaultVal, NULL);
}

TDBFieldType str2fieldType(char *str) {
    TDBFieldType type;

    if(strstr(str, "char") || strstr(str, "CHAR"))
        type = eCHAR;
    else if(strstr(str, "text") || strstr(str, "TEXT") || strstr(str, "varchar") || strstr(str, "VARCHAR"))
        type = eTEXT;
    else if(strstr(str, "int") || strstr(str, "INT") || strstr(str, "numeral") || strstr(str, "NUMERAL"))
        type = eINT;
    else if(strstr(str, "float") || strstr(str, "FLOAT"))
        type = eFLOAT;
    else if(strstr(str, "real") || strstr(str, "REAL") || strstr(str, "double") || strstr(str, "DOUBLE"))
        type = eDOUBLE;
    else if(strstr(str, "blob") || strstr(str, "BLOB"))
        type = eBUFFER;
    else {
        SHOWERROR("libmezza.dbase.TDBField", "Definição de tipos de dados incompatível")
        type = 0;
    }

    return type;
}
