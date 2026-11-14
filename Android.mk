LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := gangsterswatfix
LOCAL_SRC_FILES := src/main.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/aml-psdk
LOCAL_CPPFLAGS := -std=c++17 -fvisibility=hidden
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
