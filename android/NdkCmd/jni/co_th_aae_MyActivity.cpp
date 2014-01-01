#include <string.h>
#include <jni.h>

#include "myActivity.h"

jstring Java_co_th_aae_MyActivity_getMyData(JNIEnv* pEnv, jobject pThis)
{
  return pEnv->NewStringUTF("My Native 5:)");
}

