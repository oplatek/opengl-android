#put this only once, the first time
LOCAL_PATH:= $(call my-dir)

# First lib, from first cpp, it will be built statically:
include $(CLEAR_VARS)
LOCAL_MODULE    := mylib-first
LOCAL_SRC_FILES := first.cpp
include $(BUILD_STATIC_LIBRARY)

# Second lib, from your second cpp:
include $(CLEAR_VARS)
LOCAL_MODULE    := mylib-second
LOCAL_SRC_FILES := second.cpp
include $(BUILD_STATIC_LIBRARY)

# Third and last lib, which will depend on and include the previous ones:
include $(CLEAR_VARS)
LOCAL_MODULE    := mylib-third
LOCAL_SRC_FILES := third.cpp
LOCAL_STATIC_LIBRARIES := mylib-first mylib-second

#Finally we build our shared library including our 3 cpp:
include $(BUILD_SHARED_LIBRARY)

