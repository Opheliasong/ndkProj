LOCAL_PATH:=$(call my-dir)

# first lib, which will be built statically
#
#include $(CLEAR_VARS)
#LOCAL_MODULE    := tinyXml                  # static library �대쫫
#LOCAL_SRC_FILES := tinyxml.cpp tinyxmlparser.cpp xmltest.cpp tinyxmlerror.cpp tinystr.cpp # static library ��鍮뚮뱶���뚯뒪 �뚯씪
#include $(BUILD_STATIC_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := box2d_static
#LOCAL_SRC_FILES := lib/libbox2d.a
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/nitroFrame/include
#include $(PREBUILT_STATIC_LIBRARY)

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
/nitroFrame/TouchLayer/TouchObserver.cpp\
/nitroFrame/TouchLayer/TouchLayer.cpp\
/nitroFrame/Timer/npTimer.cpp\
/nitroFrame/Timer/npTimerObserver.cpp\
/ProjectBean/Notes/pbNoteElement.cpp\
/ProjectBean/Notes/pbTargetingNotes.cpp\
/ProjectBean/Notes/pbTabNotes.cpp\
/ProjectBean/Notes/pbDTabNotes.cpp\
/ProjectBean/Notes/pbLongPressNotes.cpp\
/ProjectBean/Notes/pbNinjaNotes.cpp\
/ProjectBean/Notes/pbNoteProcessor.cpp\
/ProjectBean/pbGlobalVariable.cpp\
/ProjectBean/pbDrawUnit.cpp\
/ProjectBean/pbBackground.cpp\
/ProjectBean/pbUI.cpp\
/ProjectBean/pbSceneWrapper.cpp\
/ProjectBean/pbMainFrame.cpp\
#/ProjectBean/pbCharacter.cpp\
#/ProjectBean/pbBoss.cpp\
/ProjectBean/pbEffect.cpp\
/ProjectBean/pbGuideLine.cpp\
/ProjectBean/pbNote.cpp\
/ProjectBean/pbNoteProcessor.cpp\

USER_SYSPATH:= /Users/nitropig/Documents/SDKs/android-ndk-r8/platforms/android-8/arch-arm/usr
#LOCAL_LDLIBS:= -L$(SYSROOT)usr/lib -llog -lEGL -lGLESv1_CM -landroid
LOCAL_LDLIBS:= -L$(SYSROOT)usr/lib -llog -lGLESv1_CM -landroid
LOCAL_LDLIBS+=-L$(SYSROOT)/usr/lib -ldl
LOCAL_LDLIBS+=-L$(SYSROOT)/usr/lib -lz
#LOCAL_LDILIBS:= $(USER_SYSPATH)/lib -llog -lEGL -lGLESv1_CM -landroid
#LOCAL_LDLIBS+=-L$(USER_SYSPATH)/lib -ldl
#LOCAL_LDLIBS+=-L$(USER_SYSPATH)/lib -lz

LOCAL_STATIC_LIBRARIES += tinyXml	libpng libstdc++ libzip
#LOCAL_EXPORT_LDLIBS := $(LOCAL_PATH)/libs/$(TARGET_ARCH_ABI)/libsupc++.a
include $(BUILD_SHARED_LIBRARY)