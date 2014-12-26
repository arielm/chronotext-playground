LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

CINDER_PATH = ../../../../../cinder

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../src
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../src/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

CHR_USE_FMOD_SOUND := 1
include $(LOCAL_PATH)/$(CINDER_PATH)/blocks/new-chronotext-toolkit/android/Android.mk

LOCAL_CFLAGS :=
LOCAL_CFLAGS += -ffast-math -O3
#LOCAL_CFLAGS += -g -DDEBUG

LOCAL_LDLIBS := -llog -landroid
LOCAL_STATIC_LIBRARIES := cinder boost_system boost_filesystem boost_thread freeimage ft2 android_native_app_glue
LOCAL_STATIC_LIBRARIES += icu-common hb
LOCAL_SHARED_LIBRARIES += fmodex

LOCAL_MODULE := TestBed2
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,cinder)
$(call import-module,boost)
$(call import-module,../../blocks/hb-icu/android)
$(call import-module,../../blocks/FMOD/android)
