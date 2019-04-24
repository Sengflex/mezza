/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   dbTDBRow.h
 * Author: root
 *
 * Created on 22 de Novembro de 2018, 15:27
 */

#ifndef MZ_DBASE_DBROW_H
#define MZ_DBASE_DBROW_H

#include "../base/TList.h"
#include "../Type.h"

#include "TDBType.h"
#include "../base/Exceptions.h"

#ifndef MZ_DBROW_IMPLEMENT
    declare_exception(ExceptionDBRowCreation)
    declare_exception(ExceptionDBRowGet)
    declare_exception(ExceptionDBRowSet)
    declare_exception(ExceptionDBRowSQLGen)

#else
    define_exception(ExceptionDBRowCreation,      "Falha em criar o objeto TDBRow")
    define_exception(ExceptionDBRowGet,      "Falha em obter os valores das celulas do registro")
    define_exception(ExceptionDBRowSet,      "Falha em atribuir os valores das celulas do registro")
    define_exception(ExceptionDBRowSQLGen,      "Composicao de comando SQL para registro")

#endif

/**
 * Cria um objeto registro que contém uma lista de valores correspondentes
 * aos campos de uma consulta/tabela
 *
 * @param memmgr Manager para alocações
 * @param cells Vetor de strings com os valores
 * @param cellsSz Tamanho do vetor cells
 *
 * @return Um objeto registro alocado e inicializado. NULL em caso de erro
 *
 * Dispara: ExceptionDBRowCreation
 * */
TDBRow *TDBRow_Create(TMemMgr *memmgr, char **cells, TSize cellsSz);
/**
 * Obt�m os valores do registro
 *
 * Os par�metros finais, cujo numero � vari�vel, ser�o ponteiros para
 * vari�veis do tipo TString. Isso permitir� que seja atribu�do a cada vari�vel
 * o endere�o dos novos objetos TString gerados para conter os valores do registro
 *
 * @param memmgr Gerenciador de mem�ria
 * @param row Objeto row (registro/linha) criado com TDBRow_Create
 * @param nofParams N�mero de par�metros vari�veis. Dever� corresponder ao n�mero de
 *                  campos no registro, mas poder� ser menor. Neste caso ser�o alocados
 *                  somente a quantidade de objetos indicada. Se for maior, ser�o alocados
 *                  somente a quantidade de objetos para o numero de campos do registro.
 *
 * @return OK em caso de sucesso. FAIL em caso de a opera��o nao poder ser realizada
 *
 * Dispara: ExceptionDBRowGet
 * */
TStatus      TDBRow_Get(TMemMgr *memmgr, TDBRow *row, TCount nofParams, ...);

/**
 * Modifica os valores para os campos do registro
 *
 * Os par�metros finais, cujo numero � vari�vel, ser�o
 * vari�veis do tipo 'char *' contendo os valores para o registro
 *
 * @param memmgr Gerenciador de mem�ria
 * @param cursor Objeto cursor criado com DBCursor_Create
 * @param nofParams N�mero de par�metros vari�veis. Dever� corresponder ao n�mero de
 *                  campos no registro, mas poder� ser menor. Neste caso ser�o modificadas
 *                  somente a quantidade de c�lulas indicada. Se for maior, ser�o modificadas
 *                  somente a quantidade de c�lulas para o numero de campos do registro.
 *
 * @return OK em caso de sucesso. FAIL em caso de a opera��o nao poder ser realizada
 *
 * Dispara: ExceptionDBRowSet
 * */
TStatus      TDBRow_Set(TMemMgr *memmgr, TDBRow *row, TCount nofParams, ...);

#endif /* MZ_DBASE_DBROW_H */
