#ifndef MZ_TMAP_GENERATOR_H
#define MZ_TMAP_GENERATOR_H

#define GENERATE_DECL_TMAP(SUFFIX, KEYTYPE, VALUETYPE) \
		VALUETYPE TMap_SetEntry##SUFFIX(TMap *map, KEYTYPE key, VALUETYPE value); \
		void      TMap_UnsetEntry##SUFFIX(TMap *map, KEYTYPE key); \
		VALUETYPE TMap_GetEntry##SUFFIX(TMap *map, KEYTYPE key); \
		TLstNod  *TMap_GetEntryNode##SUFFIX(TMap *map, KEYTYPE key); 
		
#define GENERATE_DEFINE_TMAP(SUFFIX, KEYTYPE, KEYCMP_FUNC,  VALUETYPE, VALATTRIB_FUNC, VALUEERROR, ENTRY_TYPE, ENTRY_CREATOR_FUNC) \
	VALUETYPE TMap_SetEntry##SUFFIX(TMap *map, KEYTYPE key, VALUETYPE value) { \
		TLstNod *nodeKey; \
		ENTRY_TYPE *entry; \
		 \
		nodeKey =  TMap_GetEntryNode##SUFFIX(map, key); \
		 \
		if(nodeKey) { \
			entry = (ENTRY_TYPE *)nodeKey->item; \
		} else { \
			entry = ENTRY_CREATOR_FUNC (TObject_ManagerOf(map), key, value); \
			onerror(entry) \
				throw_note(ExceptionTMapSetEntry, VALUEERROR, "Criacao de objeto de entrada") \
			onerror(TList_Add(map->entries, entry)) { \
				TObject_Destroy(entry, NULL); \
				throw_note(ExceptionTMapSetEntry, VALUEERROR, "Adicao de entrada à lista") \
			} \
		} \
		 \
		if( VALUEERROR == VALATTRIB_FUNC (entry->value, value) ) \
			throw_note(ExceptionTMapSetEntry, VALUEERROR, "Atribuição de valor à entrada") \
		 \
		map->entriesSize++; \
		 \
		return value; \
	} \
	void TMap_UnsetEntry##SUFFIX(TMap *map, KEYTYPE key) { \
		TLstNod *nodeKey; \
		 \
		nodeKey =  TMap_GetEntryNode##SUFFIX (map, key); \
		 \
		if(nodeKey) { \
			TObject_Destroy(nodeKey->item, NULL); \
			TList_Rem(map->entries, nodeKey); \
		} \
		 \
		map->entriesSize--; \
	} \
	TLstNod *TMap_GetEntryNode##SUFFIX(TMap *map, KEYTYPE key) { \
		LOOPLIST(map->entries,) \
			if(0 == KEYCMP_FUNC (((ENTRY_TYPE *)_NODE_->item)->key, key)) \
					return _NODE_ ; \
		END_LOOPLIST \
		 \
		return NULL; \
	} \
	VALUETYPE TMap_GetEntry##SUFFIX(TMap *map, KEYTYPE key) { \
		TLstNod   *nodeKey; \
		ENTRY_TYPE *entry; \
		 \
		nodeKey =  TMap_GetEntryNode##SUFFIX(map, key); \
		if(nodeKey) { \
			entry = (ENTRY_TYPE *)nodeKey->item; \
			return entry->value; \
		} \
		 \
		return VALUEERROR; \
	}

#endif /* MZ_TMAP_GENERATOR_H */
