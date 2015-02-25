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

void openmax_omx_error(struct obs_openmax *omxil, OMX_ERRORTYPE error)
{
	char *msg;
	switch(error)
	{
	case OMX_ErrorNone:                     msg = "no error";                                      break;
	case OMX_ErrorBadParameter:             msg = "bad parameter";                                 break;
	case OMX_ErrorIncorrectStateOperation:  msg = "invalid state while trying to perform command"; break;
	case OMX_ErrorIncorrectStateTransition: msg = "unallowed state transition";                    break;
	case OMX_ErrorInsufficientResources:    msg = "insufficient resource";                         break;
	case OMX_ErrorBadPortIndex:             msg = "bad port index, i.e. incorrect port";           break;
	case OMX_ErrorHardware:                 msg = "hardware error";                                break;
	default:                                msg = "(no description)";                              break;
	}
	warn("OMX Error: %s", msg);
}

// Some busy loops to verify we're running in order
void block_until_state_changed(OMX_HANDLETYPE hComponent, OMX_STATETYPE wanted_eState) {
    OMX_STATETYPE eState;
    int i = 0;
    while(i++ == 0 || eState != wanted_eState) {
        OMX_GetState(hComponent, &eState);
        if(eState != wanted_eState) {
            os_sleep_ms(10);
        }
    }
}

void block_until_port_changed(struct obs_openmax *omxil, OMX_HANDLETYPE hComponent, OMX_U32 nPortIndex, OMX_BOOL bEnabled) {
    OMX_ERRORTYPE r;
    OMX_PARAM_PORTDEFINITIONTYPE portdef;
    OMX_INIT_STRUCTURE(portdef);
    portdef.nPortIndex = nPortIndex;
    OMX_U32 i = 0;
    while(i++ == 0 || portdef.bEnabled != bEnabled) {
        if((r = OMX_GetParameter(hComponent, OMX_IndexParamPortDefinition, &portdef)) != OMX_ErrorNone) {
            error("Failed to get port definition");
        }
        if(portdef.bEnabled != bEnabled) {
            os_sleep_ms(10);
        }
    }
}

void block_until_flushed(struct obs_openmax *omxil) {
    int quit;
    while(!quit) {
        //vcos_semaphore_wait(&ctx->handler_lock);
        if(omxil->flushed) {
            omxil->flushed = 0;
            quit = 1;
        }
        //vos_semaphore_post(&ctx->handler_lock);
        if(!quit) {
            os_sleep_ms(10);
        }
    }
}
