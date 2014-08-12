LOCAL_PATH:= $(call my-dir)
  
include $(CLEAR_VARS)

MY_APP_FFMPEG_OUTPUT_PATH := $(realpath $(LOCAL_PATH)/../build/ffmpeg-armv7a/output)
MY_APP_FFMPEG_INCLUDE_PATH := $(realpath $(MY_APP_FFMPEG_OUTPUT_PATH)/include)

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/libffmpegthumbnailer/ \
	$(LOCAL_PATH)/libpng/ \
	$(MY_APP_FFMPEG_INCLUDE_PATH) \

#libpng  
LIB_PNG_SRC := \
	libpng/png.c \
	libpng/pngerror.c \
	libpng/pnggccrd.c \
	libpng/pngget.c \
	libpng/pngmem.c \
	libpng/pngpread.c \
	libpng/pngread.c \
	libpng/pngrio.c \
	libpng/pngrtran.c \
	libpng/pngrutil.c \
	libpng/pngset.c \
	libpng/pngtrans.c \
	libpng/pngvcrd.c \
	libpng/pngwio.c \
	libpng/pngwrite.c \
	libpng/pngwtran.c \
	libpng/pngwutil.c
	
#libffmpegthumbnailer
LIB_MPEG_THUMBNAILER_SRC := \
	libffmpegthumbnailer/moviedecoder.cpp \
	libffmpegthumbnailer/stringoperations.cpp \
	libffmpegthumbnailer/videothumbnailer.cpp \
	libffmpegthumbnailer/videothumbnailerc.cpp \
	libffmpegthumbnailer/filmstripfilter.cpp \
	libffmpegthumbnailer/pngwriter.cpp
	
#util
UTIL_SRC := \
	com_qianxun_kankan_util_VideoThumbnailerUtil.cpp
	
LOCAL_SRC_FILES += $(LIB_PNG_SRC)
LOCAL_SRC_FILES += $(LIB_MPEG_THUMBNAILER_SRC)
LOCAL_SRC_FILES += $(UTIL_SRC)

#LOCAL_CFLAGS += -std=c99
LOCAL_CXXFLAGS=-D__STDC_CONSTANT_MACROS
LOCAL_CPPFLAGS += -std=c++11 -fexceptions
  
LOCAL_LDLIBS := -lz -llog -landroid
LOCAL_SHARED_LIBRARIES := ffmpeg

LOCAL_MODULE := thumbnailer 
  
include $(BUILD_SHARED_LIBRARY)