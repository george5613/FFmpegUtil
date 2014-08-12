/*
 * TrackInfo.h
 *
 *  Created on: 2014-8-4
 *      Author: JieLing
 */

#ifndef TRACKINFO_H_
#define TRACKINFO_H_

namespace ffmpegvideoprobe {

class TrackInfo {
public:
	TrackInfo();
	virtual ~TrackInfo();

	int s_type;

	int s_height;
	int s_width;
	int s_duration;
	char const * s_codec_name;

	float s_frame_rate;
	int s_sample_rate;
	int s_channels;

};

} /* namespace ffmpegthumbnailer */
#endif /* TRACKINFO_H_ */
