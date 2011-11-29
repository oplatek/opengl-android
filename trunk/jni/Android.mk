LOCAL_PATH:= $(call my-dir)


# First lib, from first cpp, it will be built statically:
# include $(CLEAR_VARS)
# LOCAL_MODULE    := libEsUtils  
# LOCAL_CFLAGS    := -Werror
# LOCAL_LDLIBS    := -llog -lGLESv2
# LOCAL_SRC_FILES := esUtils.cpp
# include $(BUILD_STATIC_LIBRARY)

# Second lib
include $(CLEAR_VARS)
LOCAL_MODULE    := libnatRenderer
LOCAL_CFLAGS    := -Werror
LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_SRC_FILES := renderer.cpp
LOCAL_STATIC_LIBRARIES := libEsUtils  

include $(BUILD_SHARED_LIBRARY)



