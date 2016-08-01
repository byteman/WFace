LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
$(warning "the value of LOCAL_PATH is $(LOCAL_PATH)") 
LOCAL_MODULE := modbus
LOCAL_MODULE_FILENAME := libmodbus
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src/ $(LOCAL_PATH)


FILE_LIST := $(wildcard $(LOCAL_PATH)/src/*.c)

$(warning "the value of FILE_LIST is $(FILE_LIST)")
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)


include $(BUILD_STATIC_LIBRARY)
