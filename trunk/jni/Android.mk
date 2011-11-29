# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)


# First lib, from first cpp, it will be built statically:
include $(CLEAR_VARS)
LOCAL_MODULE    := libEsUtils  
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := esUtils.cpp
LOCAL_LDLIBS    := -llog -lGLESv2
include $(BUILD_SHARED_LIBRARY)

# Second lib, depend on and include esTransform 
include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include-all
LOCAL_MODULE    := libEsTransform
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := esTransform.cpp
LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_STATIC_LIBRARIES := libEsUtils  
include $(BUILD_SHARED_LIBRARY)

# Third which will depend on and include libEsTransform libEsUtils:
include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include-all
LOCAL_MODULE    := libRenderer
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := renderer.cpp
LOCAL_STATIC_LIBRARIES := libEsTransform libEsUtils  
LOCAL_LDLIBS    := -llog -lGLESv2
include $(BUILD_SHARED_LIBRARY)

# Fourth and last lib, which will depend on and include the previous ones:
# libEsTransform libEsUtils libRenderer
include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include-all
LOCAL_MODULE    := libnatRenderer
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := jniInterface.cpp
LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_STATIC_LIBRARIES := libEsTransform libEsUtils libRenderer 
include $(BUILD_SHARED_LIBRARY)

# not necessary? it should be good for headers LOCAL_C_INCLUDES := $(LOCAL_PATH)/include-all
