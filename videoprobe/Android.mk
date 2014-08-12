LOCAL_PATH:= $(call my-dir)
  
include $(CLEAR_VARS)

MY_APP_FFMPEG_OUTPUT_PATH := $(realpath $(LOCAL_PATH)/../build/ffmpeg-armv7a/output)
MY_APP_FFMPEG_INCLUDE_PATH := $(realpath $(MY_APP_FFMPEG_OUTPUT_PATH)/include)

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/libffmpegvideoprobe/ \
	$(MY_APP_FFMPEG_INCLUDE_PATH) \

#libffmpegvideoprobe
LIB_MPEG_VIDEOPROBE_SRC := \
	libffmpegvideoprobe/trackinfo.cpp \
	libffmpegvideoprobe/moviedecoder.cpp \
	libffmpegvideoprobe/videoprobe.cpp \
	
#util
UTIL_SRC := \
	com_qianxun_kankan_util_VideoProbeUtil.cpp \
	
LOCAL_SRC_FILES += $(LIB_MPEG_VIDEOPROBE_SRC)
LOCAL_SRC_FILES += $(UTIL_SRC)

#LOCAL_CFLAGS += -std=c99
LOCAL_CXXFLAGS=-D__STDC_CONSTANT_MACROS
LOCAL_CPPFLAGS += -std=c++11 -fexceptions
  
LOCAL_LDLIBS := -lz -llog -landroid
LOCAL_SHARED_LIBRARIES := ffmpeg

LOCAL_MODULE := videoprobe 
  
include $(BUILD_SHARED_LIBRARY)