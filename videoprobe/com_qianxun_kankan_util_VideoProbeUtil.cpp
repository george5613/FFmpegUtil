#include <com_qianxun_kankan_util_VideoProbeUtil.h>
#include <android/log.h>
#include <libffmpegvideoprobe/trackinfo.h>
#include <string>

extern "C" {
#include "libffmpegvideoprobe/videoprobe.h"
}
#define  LOG_TAG    "com_qianxun_kankan_util_VideoProbeUtil.cpp"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

using namespace std;
using namespace ffmpegvideoprobe;

void setInt(JNIEnv *env, jobject item, const char* field, jint value);
void setFloat(JNIEnv *env, jobject item, const char* field, jfloat value);
void setString(JNIEnv *env, jobject item, const char* field, const char* text);
std::string jstringTostring(JNIEnv* env, jstring jstr);

void setInt(JNIEnv *env, jobject item, const char* field, jint value) {
    jclass cls;
    jfieldID fieldId;

    /* Get a reference to item's class */
    cls = env->GetObjectClass(item);

    /* Look for the instance field s in cls */
    fieldId = env->GetFieldID(cls, field, "I");
    if (fieldId == NULL)
        return;

    env->SetIntField(item, fieldId, value);
}

void setFloat(JNIEnv *env, jobject item, const char* field, jfloat value) {
    jclass cls;
    jfieldID fieldId;

    /* Get a reference to item's class */
    cls = env->GetObjectClass(item);

    /* Look for the instance field s in cls */
    fieldId = env->GetFieldID(cls, field, "F");
    if (fieldId == NULL)
        return;

    env->SetFloatField(item, fieldId, value);
}

void setString(JNIEnv *env, jobject item, const char* field, const char* text) {
    jclass cls;
    jfieldID fieldId;
    jstring jstr;

    /* Get a reference to item's class */
    cls = env->GetObjectClass(item);

    /* Look for the instance field s in cls */
    fieldId = env->GetFieldID(cls, field, "Ljava/lang/String;");
    if (fieldId == NULL)
        return;

    /* Create a new string and overwrite the instance field */
    jstr = env->NewStringUTF(text);
    if (jstr == NULL)
        return;
    env->SetObjectField(item, fieldId, jstr);
}

std::string jstringTostring(JNIEnv* env,jstring jstr)
{
	const char *c_str = NULL;
	c_str = env->GetStringUTFChars(jstr, NULL);
	std::string stemp(c_str);
	env->ReleaseStringUTFChars(jstr, c_str);
	return stemp;
}

JNIEXPORT jint JNICALL Java_com_qianxun_kankan_util_VideoProbeUtil_readDuration
  (JNIEnv *env, jclass clazz, jstring path){
	VideoProbe videoProbe;
	int duration = 0;
	try {
		duration = videoProbe.getDuration(jstringTostring(env,path));
	} catch (exception& e) {
		return (jint)-1;
	}
	return duration;
}



JNIEXPORT jobjectArray JNICALL Java_com_qianxun_kankan_util_VideoProbeUtil_readTrackInfo
  (JNIEnv *env, jclass clazz, jstring path){
	/* get java class */
	jclass cls = env->FindClass("com/qianxun/kankan/model/TrackInfo");
	if (!cls) {
		LOGE("Failed to load class (com/qianxun/kankan/model/TrackInfo)");
		return NULL;
	}

	/* get java class contructor */
	jmethodID clsCtor = env->GetMethodID(cls, "<init>", "()V");
	if (!clsCtor) {
		LOGE("Failed to find class constructor (com/qianxun/kankan/model/TrackInfo)");
		return NULL;
	}

	TrackInfo** trackinfos = NULL;
	VideoProbe videoProbe;
	jobjectArray array = NULL;
	try {
		int trackNum = videoProbe.getTracksNum(jstringTostring(env,path));
		array = env->NewObjectArray(trackNum, cls, NULL);
		trackinfos = videoProbe.generateTrackInfo(jstringTostring(env,path));
		unsigned i;
		if (array != NULL&&trackinfos!=NULL) {
			for (i = 0; i < trackNum; ++i) {
				TrackInfo *info = trackinfos[i];
				jobject item = env->NewObject(cls, clsCtor);
				if (item == NULL)
					continue;
				env->SetObjectArrayElement(array, i, item);
				setInt(env, item, "Type", (jint)info->s_type);
				setInt(env, item, "Width", (jint)info->s_width);
				setInt(env, item, "Height", (jint)info->s_height);
				setInt(env, item, "Length", (jint)info->s_duration);
				setInt(env, item, "Samplerate", (jint)info->s_sample_rate);
				setInt(env, item, "Channels", (jint)info->s_channels);
				setFloat(env, item, "Framerate", (jfloat)info->s_frame_rate);
				setString(env, item, "Codec", info->s_codec_name);
			}
			for (i = 0; i < trackNum; ++i) {
				delete(trackinfos[i]);
			}
			delete(trackinfos);
		}
	} catch (exception& e) {
		return NULL;
	}
	return array;
}

