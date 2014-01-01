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
    return pEnv->NewStringUTF(lEntry->mValue.mString);
  return NULL;
}

void Java_com_packtpub_Store_setString(
    JNIEnv *pEnv, jobject pThis, jstring pKey, jstring pValue)
{
  const char *lStringTmp = pEnv->GetStringUTFChars(pValue,NULL);

  if (lStringTmp == NULL)
    return;

  StoreEntry *lEntry = allocateEntry(pEnv, &gStore, pKey);
  if (lEntry != NULL) {
    lEntry->mType = StoreType_String;
    jsize lStringLength = pEnv->GetStringUTFLength(pValue);
    lEntry->mValue.mString = (char *) malloc (sizeof(char) * (lStringLength + 1));
    strcpy(lEntry->mValue.mString, lStringTmp);
  }
  pEnv->ReleaseStringUTFChars(pValue, lStringTmp);
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
  jobject lColor = pEnv->NewGlobalRef(pColor);

  if (lColor == NULL)
    return;
  
  StoreEntry *lEntry = allocateEntry(pEnv, &gStore, pKey);
  
  if (lEntry != NULL) {
    lEntry->mType = StoreType_Color;
    lEntry->mValue.mColor = lColor;
  } else {
    pEnv->DeleteGlobalRef(lColor);
  }
}

/*
 * Class:     com_packtpub_Store
 * Method:    getIntegerArray
 * Signature: (Ljava/lang/String;)[I
 */
JNIEXPORT jintArray JNICALL Java_com_packtpub_Store_getIntegerArray
  (JNIEnv *pEnv, jobject pThis, jstring pKey)
{
  StoreEntry *lEntry = findEntry(pEnv, &gStore, pKey, NULL);
  if (isEntryValid(pEnv, lEntry, StoreType_IntegerArray)) {
    jintArray lJavaArray = pEnv->NewIntArray(lEntry->mLength);
    if (lJavaArray == NULL)
      return NULL;
    pEnv->SetIntArrayRegion(lJavaArray, 0, lEntry->mLength,
        lEntry->mValue.mIntegerArray);
    return lJavaArray;
  } else {
    return NULL;
  }
}

/*
 * Class:     com_packtpub_Store
 * Method:    setIntegerArray
 * Signature: (Ljava/lang/String;[I)V
 */
JNIEXPORT void JNICALL Java_com_packtpub_Store_setIntegerArray
  (JNIEnv *pEnv, jobject pThis, jstring pKey, jintArray pIntegerArray)
{
  jsize lLength = pEnv->GetArrayLength(pIntegerArray);
  int32_t *lArray = (int32_t *) malloc (lLength * sizeof(int32_t));

  pEnv->GetIntArrayRegion(pIntegerArray, 0, lLength, lArray);
  if (pEnv->ExceptionCheck()) {
    free (lArray);
    return;
  }

  StoreEntry *lEntry = allocateEntry(pEnv, &gStore, pKey);
  if (lEntry != NULL) {
    lEntry->mType = StoreType_IntegerArray;
    lEntry->mLength = lLength;
    lEntry->mValue.mIntegerArray = lArray;
  } else {
    free(lArray);
  }
}

/*
 * Class:     com_packtpub_Store
 * Method:    getColorArray
 * Signature: (Ljava/lang/String;)[Lcom/packtpub/Color;
 */
JNIEXPORT jobjectArray JNICALL Java_com_packtpub_Store_getColorArray
  (JNIEnv *pEnv, jobject pThis, jstring pKey)
{ StoreEntry *lEntry = findEntry(pEnv, &gStore, pKey, NULL);

  if (isEntryValid(pEnv, lEntry, StoreType_ColorArray)) {
    jclass lColorClass = pEnv->FindClass("com/packtpub/Color");
    if (lColorClass == NULL)
      return NULL;

    jobjectArray lJavaArray = pEnv->NewObjectArray(lEntry->mLength,
        lColorClass, NULL);
    pEnv->DeleteLocalRef(lColorClass);
    if (lJavaArray == NULL)
      return NULL;

    int32_t i;
    for (i=0; i<lEntry->mLength; ++i) {
      pEnv->SetObjectArrayElement(lJavaArray, i,
          lEntry->mValue.mColorArray[i]);
      if (pEnv->ExceptionCheck())
        return NULL;
    }

    return lJavaArray;
  }

  return NULL;
}

/*
 * Class:     com_packtpub_Store
 * Method:    setColorArray
 * Signature: (Ljava/lang/String;[Lcom/packtpub/Color;)V
 */
JNIEXPORT void JNICALL Java_com_packtpub_Store_setColorArray
  (JNIEnv *pEnv, jobject pThis, jstring pKey, jobjectArray pColorArray)
{
  jsize lLength = pEnv->GetArrayLength(pColorArray);
  jobject *lArray = (jobject *) malloc (lLength * sizeof(jobject));
  int32_t i, j;

  for (i=0; i<lLength; ++i) {
    jobject lLocalColor = pEnv->GetObjectArrayElement(pColorArray, i);

    if (lLocalColor == NULL) {
      for (j=0; j<i; ++j) {
        pEnv->DeleteGlobalRef(lArray[j]);
      }
      free(lArray);
      return;
    }

    lArray[i] = pEnv->NewGlobalRef(lLocalColor);
    if (lArray[i] == NULL) {
      for (j=0; j<i; ++j) {
        pEnv->DeleteGlobalRef(lArray[j]);
      }
      free(lArray);
      return;
    } 
    pEnv->DeleteLocalRef(lLocalColor);
  }

  StoreEntry *lEntry = allocateEntry(pEnv, &gStore, pKey);
  if (lEntry != NULL) {
    lEntry->mType = StoreType_ColorArray;
    lEntry->mLength = lLength;
    lEntry->mValue.mColorArray = lArray;
  } else {
    for (j=0; j<i; ++j) {
      pEnv->DeleteGlobalRef(lArray[j]);
    }
    free(lArray);
    return;
  }
}

