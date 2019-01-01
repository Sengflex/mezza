#define MZ_TMAP_IMPLEMENT
#include "TMap.h"

#include "../../base/TMemMgr.h"
#include "../../base/TObject.h"

void tmapCleaner(TObject obj, void *treatValueAsTObject);

void *TREAT_VALUE_AS_OBJECT = (void *)0x1;

TMap *TMap_Create(TMemMgr *memmgr) {
	TMap *map = NULL;
	
	map = TObject_Create(memmgr, sizeof(TMap), tmapCleaner);
	onerror(map) throw(ExceptionTMapCreation, NULL)
	
	map->entries = TList_Create(memmgr);
	onerror(map->entries) {
		TObject_DestroyByPassDtor(map, NULL);
		throw(ExceptionTMapCreation, NULL)
	}
	
	map->entriesSize = 0;
	
	return map;
}

void tmapCleaner(TObject obj, void *treatValueAsTObject) {
	TMap *map = (TMap *)obj;
	if(map->entries) {
		if(map->entries->start) TList_ForeachDoDestroy(map->entries, treatValueAsTObject);
		TObject_Destroy(map->entries, NULL);
	}
	
}
