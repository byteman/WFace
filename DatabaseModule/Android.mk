LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
$(warning "the value of LOCAL_PATH is $(LOCAL_PATH)") 
LOCAL_MODULE := DatabaseModule
LOCAL_MODULE_FILENAME := libDatabaseModule
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include \
					$(LOCAL_PATH)/../include/poco-deveop \
					$(LOCAL_PATH)/../Core

#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_CPPFLAGS := -std=c++11 -DMY_LOG_TAG=$(LOCAL_MODULE)

FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
$(warning "the value of FILE_LIST is $(FILE_LIST)")
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)


include $(BUILD_STATIC_LIBRARY)
