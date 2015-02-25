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

#define do_log(level, format, ...) \
	blog(level, "[openmax encoder: '%s'] " format, \
			obs_encoder_get_name(omxil->encoder), ##__VA_ARGS__)
#define error(format, ...) do_log(LOG_ERROR,   "ERROR: " format, ##__VA_ARGS__)
#define warn(format, ...)  do_log(LOG_WARNING, "WARN: "  format, ##__VA_ARGS__)
#define info(format, ...)  do_log(LOG_INFO,    "INFO: "  format, ##__VA_ARGS__)
#define debug(format, ...) do_log(LOG_DEBUG,   "DEBUG: " format, ##__VA_ARGS__)

#define OMX_INIT_STRUCTURE(a) \
    memset(&(a), 0, sizeof(a)); \
    (a).nSize = sizeof(a)
#define OMX_INIT_STRUCT_VERSION(a) \
    (a).nVersion.nVersion = OMX_VERSION; \
    (a).nVersion.s.nVersionMajor = OMX_VERSION_MAJOR; \
    (a).nVersion.s.nVersionMinor = OMX_VERSION_MINOR; \
    (a).nVersion.s.nRevision = OMX_VERSION_REVISION; \
    (a).nVersion.s.nStep = OMX_VERSION_STEP

void openmax_omx_error(struct obs_openmax *omxil, OMX_ERRORTYPE error);

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
	obs_encoder_t          *encoder;

	OMX_HANDLETYPE         *omx_component;
	OMX_BUFFERHEADERTYPE   *encoder_ppBuffer_in;
	OMX_BUFFERHEADERTYPE   *encoder_ppBuffer_out;

	DARRAY(uint8_t)        packet_data;

	uint8_t                *extra_data;
	uint8_t                *sei;

	size_t                 extra_data_size;
	size_t                 sei_size;

	os_performance_token_t *performance_token;
};