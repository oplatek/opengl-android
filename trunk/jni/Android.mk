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
LOCAL_SRC_FILES := natRenderer.cpp logFunctions.cpp
LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_STATIC_LIBRARIES := esUtils 

include $(BUILD_SHARED_LIBRARY)

