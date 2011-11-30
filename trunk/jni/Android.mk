LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := esUtils 
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := esUtils.cpp
LOCAL_LDLIBS    := -llog -lGLESv2
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := test 
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := natRenderer.cpp
LOCAL_LDLIBS    := -llog -lGLESv2

include $(BUILD_SHARED_LIBRARY)

