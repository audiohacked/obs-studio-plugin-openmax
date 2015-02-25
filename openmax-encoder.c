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
	OMX_ERRORTYPE r;
	
	/* Initialize OpenMAX IL */
	if ((r = OMX_Init()) != OMX_ErrorNone) {
		error("OMX initialization failed!");
		return NULL;
	}

	/* Initialize Component */
	if ((r = OMX_GetHandle()) != OMX_ErrorNone) {
		error("Component initialization failed!");
		return NULL;
	}
	/* Setup ports */
	OMX_PARAM_PORTDEFINITIONTYPE encoder_portdef;
	/* setup input buffer */
	/* setup output buffer */
	/* configure bitrate */
	OMX_VIDEO_PARAM_BITRATETYPE bitrate;
	OMX_INIT_STRUCTURE(bitrate);
	bitrate.eControlRate = OMX_Video_ControlRateVariable;
	bitrate.nTargetBitrate = encoder_portdef.format.video.nBitrate;
	bitrate.nPortIndex = 201;
	if((r = OMX_SetParameter(ctx.encoder, OMX_IndexParamVideoBitrate, &bitrate)) != OMX_ErrorNone) {
		omx_die(r, "Failed to set bitrate for encoder output port 201");
	}
	/* configure image format */
	OMX_VIDEO_PARAM_PORTFORMATTYPE format;
	/* switch component to idle state */
	/* enable ports */
	/* allocate input/output buffers */
	/* switch component to executing state */
	
	return omxil;
}

void obs_x264_destroy(void *data)
{
	struct obs_openmax *omxil = data;
	/* Flush buffers */
	/* Disable Ports */
	/* Free the buffers */
	/* switch component to idle then to loaded */
	/* Free the component handle */
	/* Free remaining OBS structures */
}

bool obs_x264_encode(void *data, struct encoder_frame *frame,
		struct encoder_packet *packet, bool *received_packet)
{
	struct obs_openmax *omxil = data;

	*received_packet = false;
	return false;
}
