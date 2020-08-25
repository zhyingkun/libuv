LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

UV_PROJECT_PATH := $(LOCAL_PATH)/../../..

LOCAL_MODULE := uv
LOCAL_MODULE_FILENAME := libuv

LOCAL_C_INCLUDES := $(UV_PROJECT_PATH)/libuv/include
LOCAL_C_INCLUDES += $(UV_PROJECT_PATH)/libuv/src
LOCAL_C_INCLUDES += $(UV_PROJECT_PATH)/libuv/src/unix
LOCAL_C_INCLUDES += $(UV_PROJECT_PATH)/libuv/src/unix/linux

LOCAL_CFLAGS := -std=gnu89 -D__ANDROID_API__=21 -Wall -Wextra -fvisibility=hidden -Wstrict-prototypes -Wno-unused-parameter -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112

LOCAL_SRC_FILES := $(shell bash $(LOCAL_PATH)/ListSRC.sh ${UV_PROJECT_PATH})

# LOCAL_CPPFLAGS := -std=c++11 # for cpp
# TARGET_FORMAT_STRING_CFLAGS := -Wformat # -Werror=format-security

# compiled inside flag: -Wl,--whole-archive -Wl,--no-whole-archive
# LOCAL_WHOLE_STATIC_LIBRARIES := libxxx # use with other library
# outside whole archive flag
# LOCAL_STATIC_LIBRARIES :=
# LOCAL_SHARED_LIBRARIES :=

include $(BUILD_SHARED_LIBRARY)

# search for lib Android.mk in environment NDK_MODULE_PATH (path separate with ':')
# $(call import-module,libxxx) 
