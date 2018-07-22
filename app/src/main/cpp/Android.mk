LOCAL_PATH := $(call my-dir)
LOCAL_CPP_FILES := $(shell find $(LOCAL_PATH) -name '*.cpp')

include $(CLEAR_VARS)
LOCAL_MODULE := native-lib
LOCAL_SRC_FILES := $(LOCAL_CPP_FILES:$(LOCAL_PATH)/%=%)
LOCAL_LDLIBS := -L$(LOCAL_PATH) -llog -lstdc++ -landroid -lEGL -lGLESv2

include $(BUILD_SHARED_LIBRARY)
