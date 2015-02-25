/******************************************************************************
    Copyright (C) 2015 by Sean Nelson <audiohacked@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>
#include <IL/OMX_Video.h>

/*---------------------------------------------------------------------------*/
/* Primary Functions */
const char *openmax_get_name(void);
void *openmax_create(obs_data_t *settings, obs_encoder_t *encoder);
void obs_x264_destroy(void *data);
bool obs_x264_encode(void *data, struct encoder_frame *frame,
		struct encoder_packet *packet, bool *received_packet);

/*---------------------------------------------------------------------------*/
/* Secondary and Helper Functions */

/*---------------------------------------------------------------------------*/
/* Properties UI Functions */
void openmax_defaults(obs_data_t *settings);
obs_properties_t *openmax_properties(void *unused);

/*---------------------------------------------------------------------------*/
/* "Optional" Functions */
bool obs_x264_update(void *data, obs_data_t *settings);
bool obs_x264_extra_data(void *data, uint8_t **extra_data, size_t *size);
bool obs_x264_sei(void *data, uint8_t **sei, size_t *size);
bool obs_x264_video_info(void *data, struct video_scale_info *info);

/*---------------------------------------------------------------------------*/
/* Data Structures */
struct obs_encoder_info openmax_encoder = {
	.id             = "openmax-encoder",
	.type           = OBS_VIDEO_ENCODER,
	.codec          = "h264",
	.get_name       = openmax_get_name,
	.create         = openmax_create,
	.destroy        = openmax_destroy,
	.encode         = openmax_encode,
	.update         = openmax_update,
	.get_properties = openmax_properties,
	.get_defaults   = openmax_defaults,
	.get_extra_data = openmax_extra_data,
	.get_sei_data   = openmax_sei,
	.get_video_info = openmax_video_info
};

struct obs_openmax {
	obs_encoder_t  *encoder;
	OMX_HANDLETYPE *omx_component;
};