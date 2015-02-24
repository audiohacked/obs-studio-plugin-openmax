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

static void obs_openmax_destroy(void *data)
{
	struct obs_openmax *obs_omxil = data;
	OMX_ERRORTYPE r;

	// Return the last full buffer back to the encoder component
	obs_omxil->buffer_out->nFlags = OMX_BUFFERFLAG_EOS;
	if((r = OMX_FillThisBuffer(obs_omxil->context, obs_omxil->buffer_out)) != OMX_ErrorNone) {
		warn("Failed to request filling of the output buffer on encoder output port 201");
	}

	//flush buffers
	// block_until_flushed(&ctx);
	if((r = OMX_SendCommand(obs_omxil->context, OMX_CommandFlush, 200, NULL)) != OMX_ErrorNone) {
		warn("Failed to flush buffers of encoder input port 200");
	}
	// block_until_flushed(&ctx);
	if((r = OMX_SendCommand(obs_omxil->context, OMX_CommandFlush, 201, NULL)) != OMX_ErrorNone) {
		warn("Failed to flush buffers of encoder output port 201");
	}

	//disable ports
	// block_until_port_changed(ctx.camera, 71, OMX_FALSE);
	if((r = OMX_SendCommand(obs_omxil->context, OMX_CommandPortDisable, 200, NULL)) != OMX_ErrorNone) {
		warn("Failed to disable encoder input port 200");
	}
	// block_until_port_changed(obs_omxil->context, 200, OMX_FALSE);
	if((r = OMX_SendCommand(obs_omxil->context, OMX_CommandPortDisable, 201, NULL)) != OMX_ErrorNone) {
		warn("Failed to disable encoder output port 201");
	}

	// Free all the buffers
	if((r = OMX_FreeBuffer(obs_omxil->context, 201, obs_omxil->buffer_out)) != OMX_ErrorNone) {
		warn("Failed to free buffer for encoder output port 201");
	}

	// Transition all the components to idle and then to loaded states
	if((r = OMX_SendCommand(obs_omxil->context, OMX_CommandStateSet, OMX_StateIdle, NULL)) != OMX_ErrorNone) {
		warn("Failed to switch state of the encoder component to idle");
	}
	// block_until_state_changed(obs_omxil->context, OMX_StateIdle);
	if((r = OMX_SendCommand(obs_omxil->context, OMX_CommandStateSet, OMX_StateLoaded, NULL)) != OMX_ErrorNone) {
		warn("Failed to switch state of the encoder component to loaded");
	}
	// block_until_state_changed(obs_omxil->context, OMX_StateLoaded);

	// Free the component handles
	if((r = OMX_FreeHandle(obs_omxil->context)) != OMX_ErrorNone) {
		warn("Failed to free encoder component handle");
	}

	// vcos_semaphore_delete(&ctx.handler_lock);
	if((r = OMX_Deinit()) != OMX_ErrorNone) {
		warn("OMX de-initalization failed");
	}

	if (obs_omxil) {
		os_end_high_performance(obs_omxil->performance_token);
		// clear_data(obs_omxil);
		da_free(obs_omxil->packet_data);
		bfree(obs_omxil);
	}
}
