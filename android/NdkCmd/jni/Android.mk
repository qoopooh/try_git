
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := mylib
LOCAL_SRC_FILES := co_th_aae_MyActivity.c

include $(BUILD_SHARED_LIBRARY)


