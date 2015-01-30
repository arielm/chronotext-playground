LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

CINDER_PATH = ../../../../../cinder
CHR_BLOCK = $(CINDER_PATH)/blocks/new-chronotext-toolkit

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../src
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../src/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

CHR_USE_ZFONT := 1
CHR_USE_XFONT := 1
CHR_USE_PATH := 1
CHR_USE_ATLAS := 1
include $(LOCAL_PATH)/$(CHR_BLOCK)/android/Android.mk

COMMON_SRC = ../../../../Common
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(COMMON_SRC)
LOCAL_SRC_FILES += $(COMMON_SRC)/Path/Path3D.cpp

LOCAL_CFLAGS :=
LOCAL_CFLAGS += -ffast-math -O3
#LOCAL_CFLAGS += -g -DDEBUG -DFORCE_LOG

LOCAL_LDLIBS := -llog -landroid
LOCAL_STATIC_LIBRARIES := cinder boost_system boost_filesystem boost_thread android_native_app_glue
LOCAL_STATIC_LIBRARIES += ft2 icu-common hb

LOCAL_MODULE := TestBed1
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,cinder)
$(call import-module,boost)
$(call import-module,../../blocks/hb-icu/android)