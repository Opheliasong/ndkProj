LOCAL_PATH:=$(call my-dir)

# first lib, which will be built statically
#
#include $(CLEAR_VARS)
#LOCAL_MODULE    := tinyXml                  # static library 이름
#LOCAL_SRC_FILES := tinyxml.cpp tinyxmlparser.cpp xmltest.cpp tinyxmlerror.cpp tinystr.cpp # static library 에 빌드될 소스 파일
#include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := libNitroFrame
LOCAL_PRECOMPILED_HEADER = "/nitroFrame/NitroFrame.h"
LOCAL_SRC_FILES := \
/nitroFrame/log.cpp\
/nitroFrame/npDebugUtill.cpp\
/nitroFrame/gluLookAt.cpp\
/nitroFrame/npMath.cpp\
/nitroFrame/npAudioSystem.cpp\
/nitroFrame/NitroFrame.cpp\
/nitroFrame/npEngineFrames.cpp\
/nitroFrame/xmlParser/npXMLParser.cpp\
/nitroFrame/xmlParser/npDeleterParser.cpp\
/nitroFrame/xmlParser/npInsertParser.cpp\
/nitroFrame/Render/npTextureGenerator.cpp\
/nitroFrame/Render/TextureAtlas.cpp\
/nitroFrame/Render/ScreenplayStore.cpp\
/nitroFrame/Render/Sprite.cpp\
/nitroFrame/Render/DAOInsert.cpp\
/nitroFrame/Render/DAOFactory.cpp\
/nitroFrame/Render/DAOBinder.cpp\
/nitroFrame/Render/DAODeleter.cpp\
/nitroFrame/Render/npContainerDAO.cpp\
/nitroFrame/Render/npRenderProcess.cpp\
/nitroFrame/Drawable/npDrawable.cpp\
/nitroFrame/Drawable/npTextureRect.cpp\
include $(BUILD_STATIC_LIBRARY)       

include $(CLEAR_VARS)
LOCAL_MODULE    := tinyXml
LOCAL_SRC_FILES := lib/libtinyXml.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := libpng
LOCAL_SRC_FILES := lib/libpng.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := libzip
LOCAL_SRC_FILES := lib/libzip.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

APP_ABI := armeabi-v7a
LOCAL_MODULE:= libGame
LOCAL_DEFAULT_CPP_EXTENSION := cpp    
LOCAL_CFLAGS:= -DANDROID_NDK -DCONFIG_EMBEDDED -DUSE_IND_THREAD
LOCAL_SRC_FILES:=\
npNativeEvent.cpp\
/TestUnit/HelloWorld.cpp\
/nitroFrame/log.cpp\
/nitroFrame/npDebugUtill.cpp\
/nitroFrame/gluLookAt.cpp\
/nitroFrame/npMath.cpp\
/nitroFrame/npAudioSystem.cpp\
/nitroFrame/NitroFrame.cpp\
/nitroFrame/npEngineFrames.cpp\
/nitroFrame/xmlParser/npXMLParser.cpp\
/nitroFrame/xmlParser/npDeleterParser.cpp\
/nitroFrame/xmlParser/npInsertParser.cpp\
/nitroFrame/Render/npTextureGenerator.cpp\
/nitroFrame/Render/TextureAtlas.cpp\
/nitroFrame/Render/ScreenplayStore.cpp\
/nitroFrame/Render/Sprite.cpp\
/nitroFrame/Render/DAOInsert.cpp\
/nitroFrame/Render/DAOFactory.cpp\
/nitroFrame/Render/DAOBinder.cpp\
/nitroFrame/Render/DAODeleter.cpp\
/nitroFrame/Render/npContainerDAO.cpp\
/nitroFrame/Drawable/npDrawable.cpp\
/nitroFrame/Drawable/npTextureRect.cpp\


LOCAL_LDLIBS:= -L$(SYSROOT)usr/lib -llog -lEGL -lGLESv1_CM -landroid
LOCAL_LDLIBS+=-L$(SYSROOT)/usr/lib -ldl
LOCAL_LDLIBS+=-L$(SYSROOT)/usr/lib -lz
LOCAL_STATIC_LIBRARIES += tinyXml	libpng libstdc++ libzip libNitroFrame
LOCAL_EXPORT_LDLIBS := $(LOCAL_PATH)/libs/$(TARGET_ARCH_ABI)/libsupc++.a
include $(BUILD_SHARED_LIBRARY)