/*
 * TrackInfo.cpp
 *
 *  Created on: 2014-8-4
 *      Author: JieLing
 */

#include "TrackInfo.h"

namespace ffmpegvideoprobe {

TrackInfo::TrackInfo()
: s_type(-1)
, s_height(-1)
, s_width(-1)
, s_duration(-1)
, s_codec_name(nullptr)
, s_frame_rate(0)
,s_sample_rate(-1)
,s_channels(-1)
{
}

TrackInfo::~TrackInfo() {
}

} /* namespace ffmpegvideoprobe */
