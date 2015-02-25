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

#include "openmax.h"

const char *openmax_get_name(void)
{
	return "OpenMAX IL";
}

void *openmax_create(obs_data_t *settings, obs_encoder_t *encoder)
{
	struct obs_openmax *omxil = bzalloc(sizeof(struct obs_openmax));
	omxil->encoder = encoder;

	OMX_ERRORTYPE r;
	UNUSED_PARAMETER(settings);
	
	/* Initialize OpenMAX IL */
	debug("init omx");
	if ((r = OMX_Init()) != OMX_ErrorNone) {
		error("OMX initialization failed!");
		return NULL;
	}

	/* Initialize Component */
	debug("init component");
	if ((r = OMX_GetHandle(omxil->omx_component, STRING_ENCODER, NULL, NULL)) != OMX_ErrorNone) {
		error("Component initialization failed!");
		return NULL;
	}

	/* Setup ports */
	/* setup input buffer */
	debug("setup input buffer");
	OMX_PARAM_PORTDEFINITIONTYPE input;
	OMX_INIT_STRUCTURE(input);
	input.nPortIndex = 200;
	if((r = OMX_GetParameter(omxil->omx_component, OMX_IndexParamPortDefinition, &input)) != OMX_ErrorNone) {
		error("Failed to get port definition for encoder input port 200");
	}
	input.format.video.nFrameWidth  = VIDEO_WIDTH;
	input.format.video.nFrameHeight = VIDEO_HEIGHT;
	input.format.video.xFramerate   = VIDEO_FRAMERATE << 16;
	input.format.video.nStride      = (input.format.video.nFrameWidth + input.nBufferAlignment - 1) & (~(input.nBufferAlignment - 1));
	input.format.video.eColorFormat = OMX_COLOR_FormatYUV420PackedPlanar;

	/* setup output buffer */
	debug("setup output buffer");
	OMX_PARAM_PORTDEFINITIONTYPE output;
	OMX_INIT_STRUCTURE(output);
	output.nPortIndex = 200;
	if((r = OMX_GetParameter(omxil->omx_component, OMX_IndexParamPortDefinition, &output)) != OMX_ErrorNone) {
		error("Failed to get port definition for encoder input port 200");
	}
	output.nPortIndex = 201;
	output.format.video.eColorFormat = OMX_COLOR_FormatUnused;
	output.format.video.eCompressionFormat = OMX_VIDEO_CodingAVC;
	// Which one is effective, this or the configuration just below?
	output.format.video.nBitrate     = VIDEO_BITRATE;
	if((r = OMX_SetParameter(omxil->omx_component, OMX_IndexParamPortDefinition, &output)) != OMX_ErrorNone) {
		error("Failed to set port definition for encoder output port 201");
	}

	/* configure bitrate */
	OMX_VIDEO_PARAM_BITRATETYPE bitrate;
	OMX_INIT_STRUCTURE(bitrate);
	bitrate.eControlRate = OMX_Video_ControlRateVariable;
	bitrate.nTargetBitrate = output.format.video.nBitrate;
	bitrate.nPortIndex = 201;
	if((r = OMX_SetParameter(omxil->omx_component, OMX_IndexParamVideoBitrate, &bitrate)) != OMX_ErrorNone) {
		error("Failed to set bitrate for encoder output port 201");
	}

	/* configure image format */
	OMX_VIDEO_PARAM_PORTFORMATTYPE format;
	OMX_INIT_STRUCTURE(format);
	format.nPortIndex = 201;
	format.eCompressionFormat = OMX_VIDEO_CodingAVC;
	if((r = OMX_SetParameter(omxil->omx_component, OMX_IndexParamVideoPortFormat, &format)) != OMX_ErrorNone) {
		error("Failed to set video format for encoder output port 201");
	}

	/* switch component to idle state */
	debug("switch component to idle");
	if((r = OMX_SendCommand(omxil->omx_component, OMX_CommandStateSet, OMX_StateIdle, NULL)) != OMX_ErrorNone) {
		error("Failed to switch state of the encoder component to idle");
	}
	block_until_state_changed(omxil->omx_component, OMX_StateIdle);

	/* enable ports */
	/* enable input port */
	debug("enable input port");
	if((r = OMX_SendCommand(omxil->omx_component, OMX_CommandPortEnable, 200, NULL)) != OMX_ErrorNone) {
		error("Failed to enable encoder input port 200");
	}
	block_until_port_changed(omxil, omxil->omx_component, 200, OMX_TRUE);

	/* enable output port */
	debug("enable output port");
	if((r = OMX_SendCommand(omxil->omx_component, OMX_CommandPortEnable, 201, NULL)) != OMX_ErrorNone) {
		error("Failed to enable encoder output port 201");
	}
	block_until_port_changed(omxil, omxil->omx_component, 201, OMX_TRUE);

	/* allocate input/output buffers */
	/* allocate input buffer */
	debug("allocate input buffer");
	OMX_INIT_STRUCTURE(input);
	input.nPortIndex = 200;
	if((r = OMX_GetParameter(omxil->omx_component, OMX_IndexParamPortDefinition, &input)) != OMX_ErrorNone) {
		error("Failed to get port definition for encoder input port 200");
	}
	if((r = OMX_AllocateBuffer(omxil->omx_component, &omxil->encoder_ppBuffer_in, 200, NULL, input.nBufferSize)) != OMX_ErrorNone) {
		error("Failed to allocate buffer for encoder input port 200");
	}

	/* allocate output buffer */
	debug("allocate output buffer");
	OMX_INIT_STRUCTURE(output);
	output.nPortIndex = 201;
	if((r = OMX_GetParameter(omxil->omx_component, OMX_IndexParamPortDefinition, &output)) != OMX_ErrorNone) {
		error("Failed to get port definition for encoder output port 201");
	}
	if((r = OMX_AllocateBuffer(omxil->omx_component, &omxil->encoder_ppBuffer_out, 201, NULL, output.nBufferSize)) != OMX_ErrorNone) {
		error("Failed to allocate buffer for encoder output port 201");
	}

	/* switch component to executing state */
	debug("switch component to executing state");
	if((r = OMX_SendCommand(omxil->omx_component, OMX_CommandStateSet, OMX_StateExecuting, NULL)) != OMX_ErrorNone) {
		error("Failed to switch state of the encoder component to executing");
	}
	block_until_state_changed(omxil->omx_component, OMX_StateExecuting);

	return omxil;
}

void openmax_destroy(void *data)
{
	UNUSED_PARAMETER(data);
	//struct obs_openmax *omxil = data;
	/* Flush buffers */
	/* Disable Ports */
	/* Free the buffers */
	/* switch component to idle then to loaded */
	/* Free the component handle */
	/* Free remaining OBS structures */
}

bool openmax_encode(void *data, struct encoder_frame *frame,
		struct encoder_packet *packet, bool *received_packet)
{
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(frame);
	UNUSED_PARAMETER(packet);
	//struct obs_openmax *omxil = data;
	
	*received_packet = false;
	return false;
}
