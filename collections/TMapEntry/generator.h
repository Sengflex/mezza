#ifndef MZ_TMAPENTRY_GENERATOR_H
#define MZ_TMAPENTRY_GENERATOR_H

#define GENERATE_DECL_TMAPENTRY(SUFFIX, KEYTYPE, VALUETYPE) \
		typedef struct SUFFIX { \
			KEYTYPE   key; \
			VALUETYPE value; \
		} T##SUFFIX; \
		T##SUFFIX *T##SUFFIX##_Create(TMemMgr *memmgr, KEYTYPE key, VALUETYPE value);
		
#define GENERATE_DEFINE_TMAP(SUFFIX, KEYTYPE, VALUETYPE, ENTRY_CLEANER) \
		T##SUFFIX *T##SUFFIX##_Create(TMemMgr *memmgr, KEYTYPE key, VALUETYPE value) { \
			T##SUFFIX *ret; \
			 \
			ret = TObject_Create(memmgr, sizeof(T##SUFFIX), ENTRY_CLEANER); \
			onerror(ret) \
				throw(ExceptionTMapEntryCreation, NULL) \
			 \
			ret->key = key; \
			 \
			ret->value = value; \
			 \
			return ret; \
		}
		
#endif /* MZ_TMAPENTRY_GENERATOR_H */
