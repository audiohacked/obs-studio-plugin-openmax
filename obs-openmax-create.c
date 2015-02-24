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

#include "obs-openmax.h"

extern OMX_CALLBACKTYPE obs_openmax_callbacks;

#define VIDEO_BITRATE 10000000

static void *obs_openmax_create(obs_data_t *settings, obs_encoder_t *encoder)
{
	struct obs_openmax *obs_omxil = bzalloc(sizeof(struct obs_openmax));
	OMX_ERRORTYPE r;

	if((r = OMX_Init()) != OMX_ErrorNone) {
		blog(LOG_WARNING,"OMX initalization failed");
		OMX_Deinit();
	}

	// init_component_handle("video_encode", &ctx.encoder, &ctx, &callbacks);
	r = OMX_GetHandle(obs_omxil->context, 
						(char*)obs_module_text("OMXILEncoder"),
						NULL, &obs_openmax_callbacks);

	if(r != OMX_ErrorNone) {
		warn("Failed to get handle for component %s", 
			(char*)obs_module_text("OMXILEncoder"));
	}

	info("Default port definition for encoder input port 200");
	dump_port(obs_omxil->context, 200, OMX_TRUE);
	info("Default port definition for encoder output port 201");
	dump_port(obs_omxil->context, 201, OMX_TRUE);

	// Configure video format of encoder input port
	OMX_INIT_STRUCTURE(obs_omxil->input_portdef);
	obs_omxil->input_portdef.nPortIndex = 200;
	r = OMX_GetParameter(obs_omxil->context, OMX_IndexParamPortDefinition, 
			&obs_omxil->input_portdef);
	if(r != OMX_ErrorNone) {
			warn("Failed to get port definition for encoder input port 200");
	}

	// Configure video format emitted by encoder output port
	OMX_INIT_STRUCTURE(obs_omxil->output_portdef);
	obs_omxil->output_portdef.nPortIndex = 201;
	r = OMX_GetParameter(obs_omxil->context, OMX_IndexParamPortDefinition,
		&obs_omxil->output_portdef);
	if(r != OMX_ErrorNone) {
			warn("Failed to get port definition for encoder output port 201");
	}

	obs_omxil->output_portdef.format.video.nFrameWidth =
		(int)obs_encoder_get_width(obs_omxil->encoder);

	obs_omxil->output_portdef.format.video.nFrameHeight =
		(int)obs_encoder_get_height(obs_omxil->encoder);

	obs_omxil->output_portdef.format.video.xFramerate = 60<<16;

	obs_omxil->output_portdef.format.video.nBitrate = VIDEO_BITRATE;

	r = OMX_SetParameter(obs_omxil->context, OMX_IndexParamPortDefinition,
							&obs_omxil->output_portdef);
	if(r != OMX_ErrorNone) {
		blog(LOG_WARNING, "Failed to set port definition for encoder output port 201");
	}

	// Configure bitrate
	OMX_VIDEO_PARAM_BITRATETYPE bitrate;
	OMX_INIT_STRUCTURE(bitrate);
	bitrate.eControlRate = OMX_Video_ControlRateVariable;
	bitrate.nTargetBitrate = obs_omxil->output_portdef.format.video.nBitrate;
	bitrate.nPortIndex = 201;
	r = OMX_SetParameter(obs_omxil->context, OMX_IndexParamVideoBitrate, &bitrate);
	if(r != OMX_ErrorNone) {
		blog(LOG_WARNING, "Failed to set bitrate for encoder output port 201");
	}

	// Configure format
	OMX_VIDEO_PARAM_PORTFORMATTYPE format;
	OMX_INIT_STRUCTURE(format);
	format.nPortIndex = 201;
	format.eCompressionFormat = OMX_VIDEO_CodingAVC;
	r = OMX_SetParameter(obs_omxil->context, OMX_IndexParamVideoPortFormat, &format);
	if(r != OMX_ErrorNone) {
		blog(LOG_WARNING, "Failed to set video format for encoder output port 201");
	}


	// Switch components to idle state
	// say("Switching state of the encoder component to idle...");
	r = OMX_SendCommand(obs_omxil->context, OMX_CommandStateSet, OMX_StateIdle, NULL);
	if(r != OMX_ErrorNone) {
		blog(LOG_WARNING, "Failed to switch state of the encoder component to idle");
	}
	// block_until_state_changed(obs_omxil->context, OMX_StateIdle);

	// Enable ports
	info("Enabling ports...");
	r = OMX_SendCommand(obs_omxil->context, OMX_CommandPortEnable, 200, NULL);
	if(r != OMX_ErrorNone) {
		blog(LOG_WARNING, "Failed to enable encoder input port 200");
	}
	// block_until_port_changed(obs_omxil->context, 200, OMX_TRUE);

	r = OMX_SendCommand(obs_omxil->context, OMX_CommandPortEnable, 201, NULL);
	if(r != OMX_ErrorNone) {
		warn("Failed to enable encoder output port 201");
	}
	// block_until_port_changed(obs_omxil->context, 201, OMX_TRUE);

	// Allocate encoder input and output buffer
	info("Allocating buffers...");
	obs_omxil->output_portdef.nPortIndex = 201;
	r = OMX_GetParameter(obs_omxil->context, OMX_IndexParamPortDefinition,
		&obs_omxil->output_portdef);
	if(r != OMX_ErrorNone) {
		warn("Failed to get port definition for encoder output port 201");
	}
	r = OMX_AllocateBuffer(obs_omxil->context, &obs_omxil->buffer_out, 201, 
		NULL, obs_omxil->output_portdef.nBufferSize);
	if(r != OMX_ErrorNone) {
		warn("Failed to allocate buffer for encoder output port 201");
	}

	// Switch state of the components prior to starting
	// the video capture and encoding loop
	info("Switching state of the encoder component to executing...");
	r = OMX_SendCommand(obs_omxil->context, OMX_CommandStateSet, OMX_StateExecuting, NULL);
	if(r != OMX_ErrorNone) {
		blog(LOG_WARNING, "Failed to switch state of the encoder component to executing");
	}
	// block_until_state_changed(obs_omxil->context, OMX_StateExecuting);

	info("Configured port definition for encoder input port 200");
	dump_port(obs_omxil->context, 200, OMX_FALSE);
	info("Configured port definition for encoder output port 201");
	dump_port(obs_omxil->context, 201, OMX_FALSE);
	return obs_omxil;
}
