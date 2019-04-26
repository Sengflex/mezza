#define TMAP_IMPLEMENT
#include "TMap.h"
#include "../base/TMemMgr.h"
#include "../base/TObject.h"
#include <assert.h>

static void tmapCleaner(TObject obj, void *userdata);

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
void *TMap_SetEntry__Backend(TMap *map, TString key, void *value, TBool valueIsObj) {
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
		entry = TMapEntry_Create__Backend(TObject_ManagerOf(map), key, value, valueIsObj);
		onerror(entry) {
			throw_note(ExceptionTMapSetEntry, NULL, "Criacao de objeto de entrada")
		}
		onerror(TList_AddObj(map->entries, entry)) {
			TObject_Destroy(entry, NULL);
			throw_note(ExceptionTMapSetEntry, NULL, "Adicao de entrada à lista")
		}
	}
	entry->value = value;
	
	map->entriesSize++;
	return value;
}

void TMap_UnsetEntry__Backend(TMap *map, char *key, void *userdata) {
  TLstNod *nodeKey;

	#ifdef DEBUG
		assert(map);
		assert(key);
	#endif

  nodeKey = TMap_GetEntryNode(map, key);
  if (nodeKey) {
    TList_Rem__Backend(map->entries, nodeKey, userdata);
		
  	map->entriesSize--;
	}
}

TLstNod *TMap_GetEntryNode(TMap *map, char *key) {
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
void *TMap_GetEntry(TMap *map, char *key) {
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
static void tmapCleaner(TObject obj, void *userdata) {
	TMap *map = (TMap *)obj;
	if(map->entries) {
		if(map->entries->start) 
			TList_ForeachDoDestroy(map->entries, userdata);
		TObject_Destroy(map->entries, NULL);
	}	
}

static void TMapEntry_dtor(TObject obj, void *userdata) {
  TMapEntry *entry = obj;

  if (entry->key)
    TObject_Destroy(entry->key, userdata);
	if(entry->valueIsObj == TRUE) {
		TObject_Destroy(entry->value, userdata);
	}
}
TMapEntry *TMapEntry_Create__Backend(TMemMgr *memmgr, TString key, void *value, TBool valueIsObj) {
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
		ret->valueIsObj = valueIsObj;
    
    return ret;
}
