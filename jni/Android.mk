TARGET_PLATFORM := android-8

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := FlappySomething

LOCAL_C_INCLUDES += \

LOCAL_SRC_FILES += \
EventsWrapper.cpp \
../native/GameObjectBase.cpp \
../native/FlappySomething.cpp \
../native/GameWorld.cpp \
../native/Menu.cpp \
../native/Player.cpp \
../native/Pipes.cpp \
../native/Utils.cpp \
../native/Textureloader.cpp \
../native/Collider.cpp

COMMON_CFLAGS := -Wall -DANDROID -DDISABLE_IMPORTGL

LOCAL_LDLIBS := -llog -lGLESv2 -Wl,-s

LOCAL_CPPFLAGS += -std=gnu++0x -fexceptions

LOCAL_CPP_FEATURES += exceptions

include $(BUILD_SHARED_LIBRARY)