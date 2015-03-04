LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

ifndef CINDER_PATH
    $(error CINDER_PATH MUST BE DEFINED!)
endif

###

CHR_BLOCK_PATH = $(CINDER_PATH)/blocks/new-chronotext-toolkit

CHR_USE_FMOD_SOUND := 1

include $(CHR_BLOCK_PATH)/android/Android.mk

###

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../src
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../src/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

###

COMMON_SRC = ../../../../Common
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(COMMON_SRC)

LOCAL_SRC_FILES += $(COMMON_SRC)/Tasks/HttpTask.cpp

###

LOCAL_CFLAGS += -DTARGET1 -DTARGET2
LOCAL_CFLAGS += -ffast-math -O3
#LOCAL_CFLAGS += -g -DDEBUG -DFORCE_LOG

###

LOCAL_LDLIBS := -llog -landroid
LOCAL_STATIC_LIBRARIES := cinder android_native_app_glue boost_system boost_filesystem boost_thread
LOCAL_STATIC_LIBRARIES += PocoDataSQLite PocoData PocoNetSSL PocoNet PocoCrypto PocoZip PocoUtil PocoXML PocoFoundation ssl crypto
LOCAL_SHARED_LIBRARIES += fmod

LOCAL_MODULE := TestBed2
include $(BUILD_SHARED_LIBRARY)

###

$(call import-module, android/native_app_glue)

$(call import-add-path, $(CINDER_PATH)/android/prebuilt)
$(call import-module, cinder)
$(call import-module, boost)
$(call import-module, ../../blocks/POCO/android)
$(call import-module, ../../blocks/FMOD/android)
