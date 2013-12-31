#include "com_packtpub_Store.h"
#include "store.h"

#include <stdint.h>
#include <string.h>

static Store gStore = { {}, 0 };

jint Java_com_packtpub_Store_getInteger(
    JNIEnv *pEnv, jobject pThis, jstring pKey)
{
  StoreEntry *lEntry = findEntry(pEnv, &gStore, pKey, NULL);
  if (isEntryValid(pEnv, lEntry, StoreType_Integer))
    return lEntry->mValue.mInteger;
  return 0;
}

void Java_com_packtpub_Store_setInteger(
    JNIEnv *pEnv, jobject pThis, jstring pKey, jint pValue)
{
  StoreEntry *lEntry = allocateEntry(pEnv, &gStore, pKey);

  if (lEntry != NULL) {
    lEntry->mType = StoreType_Integer;
    lEntry->mValue.mInteger = pValue;
  }
}

jstring Java_com_packtpub_Store_getString(
    JNIEnv *pEnv, jobject pThis, jstring pKey)
{
  StoreEntry *lEntry = findEntry(pEnv, &gStore, pKey, NULL);
  if (isEntryValid(pEnv, lEntry, StoreType_String))
    return (*pEnv)->NewStringUTF(pEnv, lEntry->mValue.mString);
  return NULL;
}

void Java_com_packtpub_Store_setString(
    JNIEnv *pEnv, jobject pThis, jstring pKey, jstring pValue)
{
  const char *lStringTmp = (*pEnv)->GetStringUTFChars(pEnv, pValue,NULL);

  if (lStringTmp == NULL)
    return;

  StoreEntry *lEntry = allocateEntry(pEnv, &gStore, pKey);
  if (lEntry != NULL) {
    lEntry->mType = StoreType_String;
    jsize lStringLength = (*pEnv)->GetStringUTFLength(pEnv, pValue);
    lEntry->mValue.mString = (char *) malloc (sizeof(char) * (lStringLength + 1));
    strcpy(lEntry->mValue.mString, lStringTmp);
  }
  (*pEnv)->ReleaseStringUTFChars(pEnv, pValue, lStringTmp);
}

jobject Java_com_packtpub_Store_getColor(
    JNIEnv *pEnv, jobject pThis, jstring pKey)
{
  StoreEntry *lEntry = findEntry(pEnv, &gStore, pKey, NULL);

  if (isEntryValid(pEnv, lEntry, StoreType_Color))
    return lEntry->mValue.mColor;
  return NULL;
}

void Java_com_packtpub_Store_setColor(
    JNIEnv *pEnv, jobject pThis, jstring pKey, jobject pColor)
{
  jobject lColor = (*pEnv)->NewGlobalRef(pEnv, pColor);

  if (lColor == NULL)
    return;
  
  StoreEntry *lEntry = allocateEntry(pEnv, &gStore, pKey);
  
  if (lEntry != NULL) {
    lEntry->mType = StoreType_Color;
    lEntry->mValue.mColor = lColor;
  } else {
    (*pEnv)->DeleteGlobalRef(pEnv, lColor);
  }
}

