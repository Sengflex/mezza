/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * Type.h
 *
 *  Created on: 8 de nov de 2018
 *      Author: danilo
 */

#ifndef TYPES_H_
#define TYPES_H_

typedef unsigned long int TSize;
typedef unsigned long int TId;
typedef unsigned long int TLenght;
typedef unsigned long int TCount;
typedef unsigned long int TPosition;
typedef unsigned char     TBool;
typedef unsigned int      TStatus;

/* Valores para TBool.
 * TRUE deve sempre ser DIFERENTE DE ZERO.
 * FALSE deve sempre ser IGUAL A ZERO */
#define TRUE 1
#define FALSE 0

/* Valores para TStatus */
#define OK   0
#define FAIL 1

#endif /* TYPES_H_ */
