#include <string.h>
#include <jni.h>

jstring
Java_co_th_aae_MyActivity_getMyData(JNIEnv* pEnv, jobject pThis)
{
  return (*pEnv)->NewStringUTF(pEnv, "My Native :)");
}

