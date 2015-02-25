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

#pragma once
#include <stdio.h>
#include <util/dstr.h>
#include <util/darray.h>
#include <util/platform.h>
#include <obs-module.h>

#include <OMX_Core.h>
#include <OMX_Component.h>
#include <OMX_Video.h>

/*---------------------------------------------------------------------------*/
/* Primary Functions */
extern const char *openmax_get_name(void);
extern void *openmax_create(obs_data_t *settings, obs_encoder_t *encoder);
extern void openmax_destroy(void *data);
extern bool openmax_encode(void *data, struct encoder_frame *frame,
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

extern void openmax_omx_error(struct obs_openmax *omxil, OMX_ERRORTYPE error);
extern void block_until_state_changed(OMX_HANDLETYPE hComponent, OMX_STATETYPE wanted_eState);
extern void block_until_port_changed(OMX_HANDLETYPE hComponent, OMX_U32 nPortIndex, OMX_BOOL bEnabled);
extern void block_until_flushed(struct obs_openmax *omxil);

#define VIDEO_WIDTH obs_encoder_get_width(omxil->encoder)
#define VIDEO_HEIGHT obs_encoder_get_height(omxil->encoder)
#define VIDEO_FRAMERATE 60
#define VIDEO_BITRATE 50000
#define STRING_ENCODER (OMX_STRING)obs_data_get_string(settings, "component_name")

/*---------------------------------------------------------------------------*/
/* Properties UI Functions */
extern void openmax_defaults(obs_data_t *settings);
extern obs_properties_t *openmax_properties(void *unused);

/*---------------------------------------------------------------------------*/
/* "Optional" Functions */
extern bool openmax_update(void *data, obs_data_t *settings);
extern bool openmax_extra_data(void *data, uint8_t **extra_data, size_t *size);
extern bool openmax_sei(void *data, uint8_t **sei, size_t *size);
extern bool openmax_video_info(void *data, struct video_scale_info *info);

/*---------------------------------------------------------------------------*/
/* Data Structures */

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

