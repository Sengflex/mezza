
/* 
 * Projeto Mezza Library 0.3.0
 * Copyright 2018, 2019, 2020 Danilo Marcos Pontes de Almeida
 * <paioniu@gmail.com>
 * 
 * base/Exceptions.h
 *
 *  Created on:  before v0.2.0
 *      Author: Danilo Almeida
 */

#ifndef MZ_BASE_EXCEPTIONS_H
#define MZ_BASE_EXCEPTIONS_H

#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#define CONTINUE_FLAG 1
#define _GOTEXCEPTION_FLAG 2

#define _try(ID, FLAGS, AFTER)  { \
            char   *_TRY_NOTE_= NULL; \
            char    flags = FLAGS; \
            jmp_buf continue_jmp_##ID; \
            if(Exceptions_DetectUncaught()) Exceptions_Fatal(); \
			AFTER

#define try(ID) _try(ID, 0, continue_this_##ID:)
#define try_continue(ID) _try(ID, CONTINUE_FLAG, )

#define check(ID) if((Exceptions_Detect())) { \
				flags |= _GOTEXCEPTION_FLAG; \
                if( flags & CONTINUE_FLAG ) { \
                    if(setjmp(continue_jmp_##ID)) \
                        goto handle_this_##ID; \
                    goto continue_this_##ID;\
                } \
                else \
                    goto handle_this_##ID; \
            }

#define check_note(ID, NOTE) _TRY_NOTE_ = NOTE; check(ID)

#define do_continue(ID) continue_this_##ID: {

#define end_continue(ID) if( flags & _GOTEXCEPTION_FLAG ) longjmp(continue_jmp_##ID, 1); }

#define catch(ID) goto end_catch_##ID; handle_this_##ID: \
            { \
               TException *_EXCEPTION_ = Exceptions_LastException();

#define catch_quickly(ID) goto end_catch_##ID; handle_this_##ID: \
            { \
               TException *_EXCEPTION_ = Exceptions_LastException(); \
               _EXCEPTION_->resolved = 1;

#define on(EXP) {  \
                if(_EXCEPTION_->exp == &(EXP) || &(EXP) == &Exception) { \
                    _EXCEPTION_->resolved = 1;

#define rethrow_it(RET) { Exceptions_AddException( _EXCEPTION_->exp, __FILE__, __LINE__, _TRY_NOTE_); return RET; }

#define terminate(RET) reset(); return RET;

#define end_on     } \
            }

#define end_try(ID) end_catch_##ID: if(Exceptions_DetectUncaught()) Exceptions_Fatal(); Exceptions_Reset(); } }

#define throw(EXP, RET) throw_note(EXP, RET, NULL)

#define throw_note(EXP, RET, NOTE)  { \
				char localFileName[500]; \
				strcpy(localFileName, __FILE__); \
				Exceptions_AddException(& EXP, localFileName, __LINE__, NOTE); \
				return RET; }

#define throw_continue(EXP, RET) throw_continue_note(EXP, RET, NULL)

#define throw_continue_note(EXP, RET, NOTE)  { \
				char localFileName[500]; \
				strcpy(localFileName, __FILE__); \
				Exceptions_AddException(& EXP, localFileName, __LINE__, NOTE); \
				if( flags & CONTINUE_FLAG ) { \
				    flags |= _GOTEXCEPTION_FLAG; \
                    if(setjmp(continue_jmp_##ID)) \
                        return RET; \
                    goto continue_this_##ID;\
                } \
                else \
                    return RET; }

#define throw_local(ID, EXP) throw_local_note(ID, EXP, NULL)

#define throw_local_note(ID, EXP, NOTE) { \
				char localFileName[500]; \
				strcpy(localFileName, __FILE__); \
				Exceptions_AddException(& EXP, localFileName, __LINE__, NOTE); \
				if( flags & CONTINUE_FLAG ) { \
				    flags |= _GOTEXCEPTION_FLAG; \
                    if(setjmp(continue_jmp_##ID)) \
                        goto handle_this_##ID; \
                    goto continue_this_##ID;\
                } \
                else \
                    goto handle_this_##ID; }

#define declare_exception(EXP) extern TExceptionDef EXP;

#define define_exception(EXP, MSG) TExceptionDef EXP = { #EXP ": " MSG };

#define exception_msg(EXP) EXP.msg

/* Quantas exceções a lista cabe */
#define EXPLST_SZ 50

typedef struct exception {
    char *msg;
} TExceptionDef;

typedef struct exception_event {
    TExceptionDef *exp;
    char       *file;
    char       *note;
    int         line;
    int         resolved;
} TException;


extern TException    exceptions_list[EXPLST_SZ];
extern unsigned int  exceptions_list_pointer;

extern TException    ExceptionSomeOtherOcur;

void Exceptions_Init();

void Exceptions_Finish();

void Exceptions_Fatal();

void Exceptions_ResetException(TException *e);

void Exceptions_ResetPosition(unsigned int pos);

void Exceptions_Reset();

void Exceptions_AddException(TExceptionDef *e, char *file, int line, char *note);

void Exceptions_PrintUncaught(TException *e);

void Exceptions_PrintException(TException *e);

int Exceptions_DetectUncaught();

TException *Exceptions_SearchException(TExceptionDef *edef);

TException *Exceptions_LastException();

void Exceptions_PrintExceptions();

void Exceptions_Prepare();

int Exceptions_Detect();

declare_exception(Exception)
declare_exception(ExceptionSomeOther)
declare_exception(ExceptionUnknown)
declare_exception(ExceptionBanana)

/*
 * Esquema básico de uso

try_continue(ID) ou try(ID)
    <<chamada de funcao>> check(ID)

do_continue(ID) <------|
                    | obrigatório com try_continue
end_continue(ID) <--|

catch(ID)
    on(Exception)

    end_on
end_try(ID)

 */





#endif /* MZ_BASE_EXCEPTIONS_H */
