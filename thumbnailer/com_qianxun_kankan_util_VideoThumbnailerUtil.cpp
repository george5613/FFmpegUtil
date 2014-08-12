/*
 * com_qianxun_kankan_util_VideoThumbnailerUtil.cpp
 *
 *  Created on: 2014-7-1
 *      Author: JieLing
 */

#include <com_qianxun_kankan_util_VideoThumbnailerUtil.h>
#include "libffmpegthumbnailer/videothumbnailer.h"
#include "libffmpegthumbnailer/stringoperations.h"
#include "libffmpegthumbnailer/filmstripfilter.h"
#include <string>
#include <stdexcept>
#include <jni.h>

using namespace std;
using namespace ffmpegthumbnailer;

std::string jstringTostring(JNIEnv* env, jstring jstr);

JNIEXPORT jint JNICALL Java_com_qianxun_kankan_util_VideoThumbnailerUtil_generateThumbnail(
		JNIEnv *env, jclass clazz, jstring inputFile, jstring outputFile) {
	int seekPercentage = 10;
	int thumbnailSize = 128;
	int imageQuality = 8;
	bool filmStripOverlay = false;
	bool workaroundIssues = false;
	bool maintainAspectRatio = true;
	bool smartFrameSelection = false;
	try {
		VideoThumbnailer videoThumbnailer(thumbnailSize, workaroundIssues,
				maintainAspectRatio, imageQuality, smartFrameSelection);
		videoThumbnailer.setSeekPercentage(seekPercentage);
		videoThumbnailer.generateThumbnail(jstringTostring(env, inputFile), Png,
				jstringTostring(env, outputFile));
	} catch (exception& e) {
		return (jint) -1;
	} catch (...) {
		return (jint) -1;
	}
	return (jint) 0;
}

std::string jstringTostring(JNIEnv* env,jstring jstr)
{
	const char *c_str = NULL;
	c_str = env->GetStringUTFChars(jstr, NULL);
	std::string stemp(c_str);
	env->ReleaseStringUTFChars(jstr, c_str);
	return stemp;
}

