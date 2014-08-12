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

#include "videoprobe.h"
#include "moviedecoder.h"

using namespace std;

namespace ffmpegvideoprobe {

VideoProbe::VideoProbe()
:decoder(nullptr)
{
}

VideoProbe::~VideoProbe() {
	if(decoder!=NULL){
		delete (decoder);
	}
}

int timeToSeconds(const std::string& time) {
	int hours, minutes, seconds;
	sscanf(time.c_str(), "%d:%d:%d", &hours, &minutes, &seconds);

	return (hours * 3600) + (minutes * 60) + seconds;
}

int VideoProbe::getDuration(const std::string& videoFile){
	if (decoder == NULL) {
		decoder = new MovieDecoder(videoFile);
	}
	return decoder->getDuration();
}

TrackInfo** VideoProbe::generateTrackInfo(const string& videoFile) {
	if (decoder == NULL) {
		decoder = new MovieDecoder(videoFile);
	}
	return decoder->getTracksInfo();
}

int VideoProbe::getTracksNum(const string& videoFile) {
	if (decoder == NULL) {
		decoder = new MovieDecoder(videoFile);
	}
	return decoder->getTracksNum();
}

string VideoProbe::getMimeType(const string& videoFile) {
	string extension = getExtension(videoFile);

	if (extension == "avi") {
		return "video/x-msvideo";
	} else if (extension == "mpeg" || extension == "mpg" || extension == "mpe"
			|| extension == "vob") {
		return "video/mpeg";
	} else if (extension == "qt" || extension == "mov") {
		return "video/quicktime";
	} else if (extension == "asf" || extension == "asx") {
		return "video/x-ms-asf";
	} else if (extension == "wm") {
		return "video/x-ms-wm";
	} else if (extension == "mp4") {
		return "video/x-ms-wmv";
	} else if (extension == "mp4") {
		return "video/mp4";
	} else if (extension == "flv") {
		return "video/x-flv";
	} else {
		return "";
	}
}

string VideoProbe::getExtension(const string& videoFilename) {
	string extension;
	string::size_type pos = videoFilename.rfind('.');

	if (string::npos != pos) {
		extension = videoFilename.substr(pos + 1, videoFilename.size());
	}

	return extension;
}

}

