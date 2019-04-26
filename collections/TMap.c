#define TMAP_IMPLEMENT
#include "TMap.h"
#include "../base/TMemMgr.h"
#include "../base/TObject.h"
#include <assert.h>

#define    TMapEntry_Create(MEMMGR, KEY, VAL) \
                TMapEntry_Create__Backend(MEMMGR, KEY, VAL, FALSE)
#define    TMapEntry_CreateObj(MEMMGR, KEY, VAL) \
                TMapEntry_Create__Backend(MEMMGR, KEY, VAL, TRUE)

static void TMapEntry_dtor(TObject obj, void *userdata);

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

void *Map_SetEntry__Backend(TList *map, TString key, void *value, TBool valueIsObj, void *userdata) {
	TLstNod *nodeKey;
	TMapEntry *entry;

	#ifdef DEBUG
		assert(map);
		assert(key);
	#endif

	nodeKey = Map_GetEntryNode(map, key);
	if (nodeKey) {
		entry = (TMapEntry *)nodeKey->item;
		if(entry->valueIsObj == TRUE)
			TObject_Destroy(entry->value, userdata);
		entry->value = value;
		entry->valueIsObj = valueIsObj;
	} else {
		entry = TMapEntry_Create__Backend(TObject_ManagerOf(map), key, value, valueIsObj);
		onerror(entry) {
			throw_note(ExceptionTMapSetEntry, NULL, "Criacao de objeto de entrada")
		}
		onerror(TList_AddObj(map, entry)) {
			TObject_Destroy(entry, NULL);
			throw_note(ExceptionTMapSetEntry, NULL, "Adicao de entrada à lista")
		}
	}

	return value;
}

void Map_UnsetEntry__Backend(TList *map, char *key, void *userdata) {
  TLstNod *nodeKey;

	#ifdef DEBUG
		assert(map);
		assert(key);
	#endif

  nodeKey = Map_GetEntryNode(map, key);
  if (nodeKey)
    TList_Rem__Backend(map, nodeKey, userdata);
}

int map_tlistcallback(TLstNod *node, void *key) {
	if (0 == strcmp(((TMapEntry *)node->item)->key, key)) 
			return 1;
	return 0;
}

void *Map_GetEntry(TList *map, char *key) {
  TLstNod *nodeKey;

	#ifdef DEBUG
		assert(map);
		assert(key);
	#endif

  nodeKey = Map_GetEntryNode(map, key);
  if (nodeKey)
    return ((TMapEntry *)nodeKey->item)->value;
		
  return NULL;
}

static void TMapEntry_dtor(TObject obj, void *userdata) {
  TMapEntry *entry = obj;

  if (entry->key)
    TObject_Destroy(entry->key, userdata);
	if(entry->valueIsObj == TRUE) {
		TObject_Destroy(entry->value, userdata);
	}
}
