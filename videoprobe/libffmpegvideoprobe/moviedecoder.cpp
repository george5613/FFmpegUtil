//    Copyright (C) 2010 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "moviedecoder.h"

#include <stdexcept>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <android/log.h>

#define  LOG_TAG    "moviedecoder.cpp"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

using namespace std;

namespace ffmpegvideoprobe {

MovieDecoder::MovieDecoder(const string& filename) :
		m_pFormatContext(nullptr), m_FormatContextWasGiven(false) {
	initialize(filename);
}

MovieDecoder::~MovieDecoder() {
	destroy();
}

void MovieDecoder::initialize(const string& filename) {
	av_register_all();
	avcodec_register_all();
	avformat_network_init();

	string inputFile = filename == "-" ? "pipe:" : filename;
	if ((!m_FormatContextWasGiven)
			&& avformat_open_input(&m_pFormatContext, inputFile.c_str(),
					nullptr, nullptr) != 0) {
		destroy();
		throw logic_error(string("Could not open input file: ") + filename);
	}

	if (avformat_find_stream_info(m_pFormatContext, nullptr) < 0) {
		destroy();
		throw logic_error(string("Could not find stream information"));
	}

	initializeVideo();
}

void MovieDecoder::destroy() {
	if ((!m_FormatContextWasGiven) && m_pFormatContext) {
		avformat_close_input(&m_pFormatContext);
	}

	avformat_network_deinit();
}

void MovieDecoder::initializeVideo() {
	m_VideoStream = m_pFormatContext->nb_streams;

	if (m_VideoStream == -1) {
		throw logic_error("no stream found");
	}

}

int MovieDecoder::getTracksNum() {
	return m_VideoStream;
}

int MovieDecoder::getDuration(){
	return static_cast<int>(m_pFormatContext->duration / AV_TIME_BASE);
}

TrackInfo** MovieDecoder::getTracksInfo() {
	TrackInfo** infos = new TrackInfo*[m_VideoStream];

	for (unsigned int i = 0; i < m_pFormatContext->nb_streams; ++i) {
		AVCodecContext *streamCodecContext = m_pFormatContext->streams[i]->codec;
		if (streamCodecContext != NULL) {
			TrackInfo *info = new TrackInfo();
			info->s_type = streamCodecContext->codec_type;
			info->s_width = streamCodecContext->width;
			info->s_height = streamCodecContext->height;
			if(m_pFormatContext->streams[i]->avg_frame_rate.den!=0){
				info->s_frame_rate = m_pFormatContext->streams[i]->avg_frame_rate.num/(float)m_pFormatContext->streams[i]->avg_frame_rate.den;
			}
			info->s_sample_rate = streamCodecContext->sample_rate;
			info->s_channels = streamCodecContext->channels;
			info->s_duration = static_cast<int>(m_pFormatContext->duration
					/ AV_TIME_BASE);
			AVCodec *streamCodec = avcodec_find_decoder(
					streamCodecContext->codec_id);
			if (streamCodec != NULL) {
				info->s_codec_name = streamCodec->long_name;
			} else {
				streamCodecContext = nullptr;
				throw logic_error("Video Codec not found");
			}

			infos[i] = info;
		}
	}
	return infos;
}

}

