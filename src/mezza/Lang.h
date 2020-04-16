
/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * Lang.h
 *
 *  Created on: 16 de nov de 2018
 *      Author: danilo
 */

#ifndef LANG_H_
#define LANG_H_

#define LOOP(TIMES) { \
                while(TIMES > 0) { \
                    --(TIMES);

#define LOOP_END } \
                }

/**
 * Realiza repetições iterando através de um contador. O contador é inicializado em ZERO
 *
 * @param COUNT Contador. Variável de tipo inteira.
 * @param LIMIT Limite para o contador.
 * */
#define loop_i(TIMES) { \
                TPosition _INDEX_ = 0; \
                while(_INDEX_ < TIMES) {

#define end_loop_i ++_INDEX_; } \
                }
/**
 * Realiza repetições de trás para frente. TIMES é decrescentado até -1
 *
 * @param TIMES Número de vezes que o loop deverá executar. Deve ser setado previamente
*/
#define LOOP_INDEX_REVERSE(TIMES) { \
                TPosition _INDEX_ = TIMES; \
                while(_INDEX_ > 0) { \
                    --_INDEX_;

#define LOOP_INDEX_REVERSE_END } \
                }

#define PRIVATE static

#endif /* LANG_H_ */
