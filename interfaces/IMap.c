#include "IMap.h"

void tmapCleaner(TObject obj, void *extra);

IMap *IMap_Create(TMemMgr *memmgr) {
	IMap *ret;
	
	ret = TObject_Create(memmgr, sizeof(IMap), tmapCleaner);
	onerror(ret)
		throw(ExceptionTMapCreation, NULL)
	
	ret->entries = TList_Create(memmgr);
	onerror(ret->entries) {
		TObject_DestroyByPassDtor(ret, NULL);
		throw(ExceptionTMapCreation, NULL)
	}
	
	ret->entriesSize = 0;
	
	return ret;
}

void tmapCleaner(TObject obj, void *extra) {
	TMap *map = (TMap*)obj;
	
	TList_ForeachDoDestroy(map->entries);
	TObject_Destroy(map->entries, NULL);
}
