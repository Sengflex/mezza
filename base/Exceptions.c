#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "Exceptions.h"
#include "TMemMgr.h"
#include "../OnError.h"

TException    exceptions_list[EXPLST_SZ];
unsigned int  exceptions_list_pointer = 0;


define_exception(Exception, "Exceção detectada!")
define_exception(ExceptionSomeOther, "... others exceptions ocurred mean time")
define_exception(ExceptionUnknown, "Ocorreu uma exceção desconhecida!")
define_exception(ExceptionBanana, "Pisei numa casca de banana!!!")

TException    ExceptionSomeOtherOcur = { &ExceptionSomeOther, NULL, 0, 0 };

TMemMgr ExceptionsMemMgr;


void Exceptions_Init() {
    for(; exceptions_list_pointer<EXPLST_SZ; exceptions_list_pointer++) {
        exceptions_list[exceptions_list_pointer].exp = NULL;
        exceptions_list[exceptions_list_pointer].file = NULL;
        exceptions_list[exceptions_list_pointer].line = 0;
    }
    exceptions_list_pointer = 0;
    TMemMgr_Init(&ExceptionsMemMgr);
}
void Exceptions_Finish() {
    Exceptions_Reset();
    TMemMgr_FreeAll(&ExceptionsMemMgr);
}
void Exceptions_Fatal() {
    fprintf(stderr, "Exiting because of unmanageable error!\n");
    Exceptions_Finish();
    exit(1);
}
void Exceptions_ResetException(TException *e) {
    e->exp = NULL;
    if(e->file) {
        TMemMgr_Free(&ExceptionsMemMgr, e->file);
        e->file = NULL;
    }
    if(e->note) {
        TMemMgr_Free(&ExceptionsMemMgr, e->note);
        e->note = NULL;
    }
    e->line = 0;
    e->resolved = 0;
}
void Exceptions_ResetPosition(unsigned int pos) {
    Exceptions_ResetException(&exceptions_list[pos]);
}
void Exceptions_Reset() {
    exceptions_list_pointer = 0;
    for(; exceptions_list_pointer<EXPLST_SZ; exceptions_list_pointer++)
        Exceptions_ResetPosition(exceptions_list_pointer);
    exceptions_list_pointer = 0;
}
void Exceptions_AddException(TExceptionDef *e, char *file, int line, char *note) {
    TException *find = Exceptions_SearchException(e);

    if(find) {
        printf("Trying to throw an already registereed exception!\n");
        printf("\tNew ocurrence: Exception: %s\n\tAt %s(%d)\n", e->msg, file, line);
        printf("\tOld ocurrence: "); Exceptions_PrintException(find);
        Exceptions_Fatal();
    }
    if(exceptions_list_pointer==EXPLST_SZ){
        exceptions_list_pointer--;
        Exceptions_ResetPosition(exceptions_list_pointer-1);
        Exceptions_ResetPosition(exceptions_list_pointer-2);
        exceptions_list[exceptions_list_pointer-2] = ExceptionSomeOtherOcur;
        exceptions_list[exceptions_list_pointer-1] = exceptions_list[exceptions_list_pointer];
    }

    if(file) {
        exceptions_list[exceptions_list_pointer].file = TMemMgr_CreateString(&ExceptionsMemMgr,file, 0);
        PROTECT(exceptions_list[exceptions_list_pointer].file,
                "libemezza.Exceptions.AddException",
                "Falha em înicializar os dados da exceção lancada",
                Exceptions_Fatal();)
    }

    if(note) {
    	exceptions_list[exceptions_list_pointer].note = TMemMgr_CreateString(&ExceptionsMemMgr,note, 0);
    	    PROTECT(exceptions_list[exceptions_list_pointer].note,
    	            "libemezza.Exceptions.AddException",
    	            "Falha em înicializar os dados da exceção lancada",
    	            TMemMgr_Free(&ExceptionsMemMgr, exceptions_list[exceptions_list_pointer].file);
    	            Exceptions_Fatal();)
    }


    exceptions_list[exceptions_list_pointer].exp = e;
    exceptions_list[exceptions_list_pointer].line = line;
    exceptions_list[exceptions_list_pointer].resolved = 0;
    exceptions_list_pointer++;
}

void Exceptions_PrintUncaught(TException *e) {
    fprintf(stderr, "Uncaught "); Exceptions_PrintException(e);
    fprintf(stderr, "Exception Stack: \n");
    Exceptions_PrintExceptions();
}

void Exceptions_PrintException(TException *e) {
    if(!e)
        e = Exceptions_LastException();

    if(!e) return;

    fprintf(stderr, "Exception%s: %s\n\tAt %s(%d)\n",
        e->resolved ? "[SOLVED]" : "",
        e->exp->msg, e->file, e->line);
	if(e->note) {
		fprintf(stderr, "\t%s\n", e->note);
	}

}

int Exceptions_DetectUncaught() {
    TException *_EXCEPTION_ = Exceptions_LastException();

    if(_EXCEPTION_ && !_EXCEPTION_->resolved) {
        Exceptions_PrintUncaught(_EXCEPTION_);
        return 1;
    }
    return 0;
}
TException *Exceptions_SearchException(TExceptionDef *edef) {
    int i;
    for(i = 0; i<exceptions_list_pointer; i++) {
        if(exceptions_list[i].exp == edef && !exceptions_list[i].resolved)
            return &exceptions_list[i];
    }
    return NULL;
}

TException *Exceptions_LastException() {
    if(exceptions_list_pointer) {
        return &exceptions_list[exceptions_list_pointer-1];
    }
    return NULL;
}
void Exceptions_PrintExceptions() {
    unsigned int i;

    for(i=0; i<exceptions_list_pointer; i++)
        fprintf(stderr, "\tE: %s\n\t%s(%d)\n", exceptions_list[i].exp->msg, exceptions_list[i].file, exceptions_list[i].line);
}

int Exceptions_Detect() {
    return exceptions_list_pointer;
}

