#define TMAP_IMPLEMENT
#include "TMap.h"
#include "../base/TMemMgr.h"
#include "../base/TObject.h"
#include <assert.h>

static void tmapCleaner(TObject obj, void *treatValueAsTObject);

void *TREAT_VALUE_AS_OBJECT = (void *)0x1;

TMap *TMap_Create(TMemMgr *memmgr) {
	TMap *map = NULL;

	#ifdef DEBUG
		assert(memmgr);
	#endif
	
	map = TObject_Create(memmgr, sizeof(TMap), tmapCleaner);
	onerror(map) 
		throw(ExceptionTMapCreation, NULL)
	
	map->entries = TList_Create(memmgr);
	onerror(map->entries) {
		TObject_DestroyByPassDtor(map, NULL);
		throw(ExceptionTMapCreation, NULL)
	}
	
	map->entriesSize = 0;
	
	return map;
}
void *TMap_SetEntry(TMap *map, TString key, void *value) {
	TLstNod *nodeKey;
	TMapEntry *entry;

	#ifdef DEBUG
		assert(map);
		assert(key);
	#endif

	nodeKey = TMap_GetEntryNode(map, key);
	if (nodeKey) {
		entry = (TMapEntry *)nodeKey->item;
	} else {
		entry = TMapEntry_Create(TObject_ManagerOf(map), key, value);
		onerror(entry) {
			throw_note(ExceptionTMapSetEntry, NULL, "Criacao de objeto de entrada")
		}
		onerror(TList_Add(map->entries, entry)) {
			TObject_Destroy(entry, NULL);
			throw_note(ExceptionTMapSetEntry, NULL, "Adicao de entrada à lista")
		}
	}
	entry->value = value;
	
	map->entriesSize++;
	return value;
}
void TMap_UnsetEntry(TMap *map, TString key) {
  TLstNod *nodeKey;

	#ifdef DEBUG
		assert(map);
		assert(key);
	#endif

  nodeKey = TMap_GetEntryNode(map, key);
  if (nodeKey) {
    TObject_Destroy(nodeKey->item, NULL);
    TList_Rem(map->entries, nodeKey);
  }
  map->entriesSize--;
}
TLstNod *TMap_GetEntryNode(TMap *map, TString key) {
	#ifdef DEBUG
		assert(map);
		assert(key);
	#endif

  LOOPLIST(map->entries, )
  	if (0 == strcmp(((TMapEntry *)_NODE_->item)->key, key)) 
			return _NODE_;
  END_LOOPLIST 
	
	return NULL;
}
void *TMap_GetEntry(TMap *map, TString key) {
  TLstNod *nodeKey;

	#ifdef DEBUG
		assert(map);
		assert(key);
	#endif

  nodeKey = TMap_GetEntryNode(map, key);
  if (nodeKey)
    return ((TMapEntry *)nodeKey->item)->value;
		
  return NULL;
}
static void tmapCleaner(TObject obj, void *treatValueAsTObject) {
	TMap *map = (TMap *)obj;
	if(map->entries) {
		if(map->entries->start) 
			TList_ForeachDoDestroy(map->entries, treatValueAsTObject);
		TObject_Destroy(map->entries, NULL);
	}	
}

static void TMapEntry_dtor(TObject obj, void *treatValueAsTObject) {
  TMapEntry *entry = obj;

  if (entry->key)
    TObject_Destroy(entry->key, NULL);
	if((long)treatValueAsTObject) {
		TObject_Destroy(entry->value, NULL);
	}
}

TMapEntry *TMapEntry_Create(TMemMgr *memmgr, TString key, void *value) {
    TMapEntry *ret;

    #ifdef DEBUG
        assert(memmgr);
        assert(key);
    #endif

    ret = TObject_Create(memmgr, sizeof(TMapEntry), TMapEntry_dtor);
    onerror(ret) 
        throw(ExceptionTMapEntryCreation, NULL) 
        
    ret->key = key;
    ret->value = value;
    
    return ret;
}
