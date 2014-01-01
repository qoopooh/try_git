#include "store.h"

int32_t isEntryValid(JNIEnv *pEnv, StoreEntry *pEntry, StoreType pType)
{
  if (pEntry == NULL)
    throwNotExistingKeyException(pEnv);
  else if (pEntry->mType != pType)
    throwInvalidTypeException(pEnv);
  else
    return 1;
  return 0;
}

StoreEntry *allocateEntry(JNIEnv *pEnv, Store *pStore, jstring pKey) {
  int32_t lError = 0;
  StoreEntry *lEntry = findEntry(pEnv, pStore, pKey, &lError);
  
  if (lEntry != NULL) {
    releaseEntryValue(pEnv, lEntry);
  } else if (!lError) {
    if (pStore->mLength >= STORE_MAX_CAPACITY) {
      throwStoreFullException(pEnv);
      return NULL;
    }
    lEntry = pStore->mEntries + pStore->mLength;
    const char *lKeyTmp = pEnv->GetStringUTFChars(pKey, NULL);
    if (lKeyTmp == NULL)
      return NULL;
    lEntry->mKey = (char*) malloc(strlen(lKeyTmp));
    strcpy(lEntry->mKey, lKeyTmp);
    pEnv->ReleaseStringUTFChars(pKey, lKeyTmp);
    ++pStore->mLength;
  }
  return lEntry;
}

StoreEntry *findEntry(JNIEnv *pEnv, Store *pStore, jstring pKey, int32_t *pError)
{
  StoreEntry *lEntry = pStore->mEntries;
  StoreEntry *lEntryEnd = lEntry + pStore->mLength;

  const char *lKeyTmp = pEnv->GetStringUTFChars(pKey, NULL);
  
  if (lKeyTmp == NULL) {
    if (pError != NULL) {
      *pError = 1;
    }
    return NULL;
  }
  while ((lEntry < lEntryEnd) && (strcmp(lEntry->mKey, lKeyTmp) != 0))
    ++lEntry;
  pEnv->ReleaseStringUTFChars(pKey, lKeyTmp);
  return (lEntry == lEntryEnd) ? NULL:lEntry;
}

void releaseEntryValue(JNIEnv *pEnv, StoreEntry *pEntry)
{
  int32_t i;

  switch (pEntry->mType) {
    case StoreType_String:
      free(pEntry->mValue.mString);
      break;
    case StoreType_Color:
      pEnv->DeleteGlobalRef(pEntry->mValue.mColor);
      break;
    case StoreType_IntegerArray:
      free(pEntry->mValue.mIntegerArray);
      break;
    case StoreType_ColorArray:
      for (i=0; i<pEntry->mLength; i++) {
        pEnv->DeleteGlobalRef(pEntry->mValue.mColorArray[i]);
      }
      free(pEntry->mValue.mColorArray);
      break;
    default:
      break;
  }
}

void throwInvalidTypeException(JNIEnv *pEnv)
{
  jclass cls = pEnv->FindClass("com/packtpub/exception/InvalidTypeException");
  if (cls != NULL)
    pEnv->ThrowNew(cls, "Key does not exist.");
  pEnv->DeleteLocalRef(cls);
}

void throwNotExistingKeyException(JNIEnv *pEnv)
{
  jclass cls = pEnv->FindClass("com/packtpub/exception/NotExistingKeyException");
  if (cls != NULL)
    pEnv->ThrowNew(cls, "There is no key");
  pEnv->DeleteLocalRef(cls);
}

void throwStoreFullException(JNIEnv *pEnv)
{
  jclass cls = pEnv->FindClass("com/packtpub/exception/StoreFullException");
  if (cls != NULL)
    pEnv->ThrowNew(cls, "FULL storage");
  pEnv->DeleteLocalRef(cls);
}

