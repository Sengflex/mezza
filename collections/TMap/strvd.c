#include "strvd.h"

#include "../../base/TMemMgr.h"
#include "../../base/TMemMgr.h"
#include "../../base/TObject.h"
#include "../TMapEntry/strvd.h"

void *valattrib_strvd(void **value1, void *value2) { return *value1 = value2; }

void *TMap_SetEntry_Strvd(TMap *map, TString key, void *value) {
  TLstNod *nodeKey;
  TMapEntryStrVoid *entry;
  nodeKey = TMap_GetEntryNode_Strvd(map, key);
  if (nodeKey) {
    entry = (TMapEntryStrVoid *)nodeKey->item;
  } else {
    entry = TMapEntryStrVoid_Create(TObject_ManagerOf(map), key, value);
    onerror(entry)
        throw_note(ExceptionTMapSetEntry, NULL, "Criacao de objeto de entrada")
            onerror(TList_Add(map->entries, entry)) {
      TObject_Destroy(entry, NULL);
      throw_note(ExceptionTMapSetEntry, NULL, "Adicao de entrada à lista")
    }
  }
  if (NULL == valattrib_strvd(&entry->value, value))
    throw_note(ExceptionTMapSetEntry, NULL, "Atribuição de valor à entrada")
        map->entriesSize++;
  return value;
}
void TMap_UnsetEntry_Strvd(TMap *map, TString key) {
  TLstNod *nodeKey;
  nodeKey = TMap_GetEntryNode_Strvd(map, key);
  if (nodeKey) {
    TObject_Destroy(nodeKey->item, NULL);
    TList_Rem(map->entries, nodeKey);
  }
  map->entriesSize--;
}
TLstNod *TMap_GetEntryNode_Strvd(TMap *map, TString key) {
  LOOPLIST(map->entries, )
  if (0 == strcmp(((TMapEntryStrVoid *)_NODE_->item)->key, key)) return _NODE_;
  END_LOOPLIST return NULL;
}
void *TMap_GetEntry_Strvd(TMap *map, TString key) {
  TLstNod *nodeKey;
  TMapEntryStrVoid *entry;
  nodeKey = TMap_GetEntryNode_Strvd(map, key);
  if (nodeKey) {
    entry = (TMapEntryStrVoid *)nodeKey->item;
    return entry->value;
  }
  return NULL;
}
