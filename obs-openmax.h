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

#include <stdio.h>
#include <util/dstr.h>
#include <util/darray.h>
#include <util/platform.h>
#include <obs-module.h>

#include <OMX_Core.h>
#include <OMX_Component.h>
#include <OMX_Video.h>

#define do_log(level, format, ...) \
	blog(level, "[openmax encoder: '%s'] " format, \
			obs_encoder_get_name(obs_omxil->encoder), ##__VA_ARGS__)

#define warn(format, ...)  do_log(LOG_WARNING, format, ##__VA_ARGS__)
#define info(format, ...)  do_log(LOG_INFO,    format, ##__VA_ARGS__)
#define debug(format, ...) do_log(LOG_DEBUG,   format, ##__VA_ARGS__)

// Dunno where this is originally stolen from...
#define OMX_INIT_STRUCTURE(a) \
    memset(&(a), 0, sizeof(a)); \
    (a).nSize = sizeof(a); \
    (a).nVersion.nVersion = OMX_VERSION; \
    (a).nVersion.s.nVersionMajor = OMX_VERSION_MAJOR; \
    (a).nVersion.s.nVersionMinor = OMX_VERSION_MINOR; \
    (a).nVersion.s.nRevision = OMX_VERSION_REVISION; \
    (a).nVersion.s.nStep = OMX_VERSION_STEP

/* ------------------------------------------------------------------------- */
extern void dump_port(OMX_HANDLETYPE hComponent, OMX_U32 nPortIndex, OMX_BOOL dumpformats);
/* ------------------------------------------------------------------------- */
/*
	OMX Sources: OMX.r600.video_encoder.avc
				 OMX.radeonsi.video_encoder.avc
*/
struct obs_openmax {
	obs_encoder_t                *encoder;

	DARRAY(uint8_t)              packet_data;

	uint8_t                      *extra_data;
	uint8_t                      *sei;

	size_t                       extra_data_size;
	size_t                       sei_size;

	OMX_PARAM_PORTDEFINITIONTYPE input_portdef;
	OMX_PARAM_PORTDEFINITIONTYPE output_portdef;
	OMX_BUFFERHEADERTYPE         *buffer_in;
	OMX_BUFFERHEADERTYPE         *buffer_out;
	OMX_HANDLETYPE               *context;

	os_performance_token_t       *performance_token;
};