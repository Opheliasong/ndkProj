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
/ProjectBean/Notes/pbNoteElement.cpp\
/nitroFrame/TouchLayer/TouchObserver.cpp\
/nitroFrame/TouchLayer/TouchLayer.cpp\
/nitroFrame/Timer/npTimerObserver.cpp\
/nitroFrame/Timer/npTimer.cpp\
/ProjectBean/pbStageValue.cpp\
/ProjectBean/Scene/pbBasicDrawUnit.cpp\
/ProjectBean/Scene/pbMainFrame.cpp\
/ProjectBean/Scene/pbSceneNavigator.cpp\
/ProjectBean/Scene/pbBackground.cpp\
/ProjectBean/Scene/pbUI.cpp\
/ProjectBean/Scene/pbEffect.cpp\
/ProjectBean/Scene/ResultScene/pbResultViewer.cpp\
/ProjectBean/Scene/ShopScene/pbVehicle.cpp\
/ProjectBean/Scene/ShopScene/pbInventory.cpp\
/ProjectBean/Scene/ShopScene/pbGoldPouch.cpp\
/ProjectBean/Scene/ShopScene/pbItem.cpp\
/ProjectBean/Scene/ShopScene/pbShop.cpp\
/ProjectBean/Scene/pbSceneWrapper.cpp\
/ProjectBean/Notes/pbNotePatternGroup.cpp\
/ProjectBean/Notes/pbNotePatternGenerator.cpp\
/ProjectBean/Notes/pbTargetingNotes.cpp\
/ProjectBean/Notes/pbDTabNotes.cpp\
/ProjectBean/Notes/pbLongPressNotes.cpp\
/ProjectBean/Notes/pbNinjaNotes.cpp\
/ProjectBean/Notes/pbTabNotes.cpp\
/ProjectBean/Notes/pbNoteProcessor.cpp\
/ProjectBean/Trigger/pbStageTrigger.cpp\
/ProjectBean/Trigger/pbTimeTrigger.cpp\
/ProjectBean/Actor/pbHPMeter.cpp\
/ProjectBean/Actor/pbTargetStamp.cpp\
/ProjectBean/Actor/pbMarionette.cpp\
/ProjectBean/Actor/pbComboManager.cpp\
/ProjectBean/Actor/pbCharacter.cpp\
/ProjectBean/Actor/pbBoss.cpp\
#/ProjectBean/pbNote.cpp\
/ProjectBean/pbNoteProcessor.cpp\
/ProjectBean/pbGuideLine.cpp\


LOCAL_LDLIBS:= -L$(SYSROOT)usr/lib -llog -lEGL -lGLESv1_CM -landroid
LOCAL_LDLIBS+=-L$(SYSROOT)/usr/lib -ldl
LOCAL_LDLIBS+=-L$(SYSROOT)/usr/lib -lz
LOCAL_STATIC_LIBRARIES += tinyXml	libpng libstdc++ libzip
LOCAL_EXPORT_LDLIBS := $(LOCAL_PATH)/libs/$(TARGET_ARCH_ABI)/libsupc++.a
include $(BUILD_SHARED_LIBRARY)