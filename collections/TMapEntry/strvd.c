
#include "TMapEntry.h"
#include "strvd.h"

#include "strvd.h"

#include "../../OnError.h"
#include "../../base/TObject.h"

static void TMapEntryStrVoid_cleaner(TObject obj, void *treatValueAsTObject) {
  TMapEntryStrVoid *entry = obj;

  if (entry->key)
    TObject_Destroy(entry->key, NULL);
	if((int)treatValueAsTObject) {
		TObject_Destroy(entry->value, NULL);
	}
}

TMapEntryStrVoid *TMapEntryStrVoid_Create(TMemMgr *memmgr, TString key,
                                            void *value) {
  TMapEntryStrVoid *ret;
  ret = TObject_Create(memmgr, sizeof(TMapEntryStrVoid),
                       TMapEntryStrVoid_cleaner);
  onerror(ret) throw(ExceptionTMapEntryCreation, NULL) ret->key = key;
  ret->value = value;
  return ret;
}
