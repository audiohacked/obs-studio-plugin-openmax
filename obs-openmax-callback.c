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
/** Callbacks implementation of the video encoder component*/
OMX_ERRORTYPE obs_openmax_eventhandler(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_EVENTTYPE eEvent,
  OMX_U32 Data1,
  OMX_U32 Data2,
  OMX_PTR pEventData) {

  OMX_ERRORTYPE err = OMX_ErrorNone;
#if 0

  DEBUG(DEB_LEV_SIMPLE_SEQ, "Hi there, I am in the %s callback\n", __func__);
  if(eEvent == OMX_EventCmdComplete) {
    if (Data1 == OMX_CommandStateSet) {
      DEBUG(DEB_LEV_SIMPLE_SEQ, "State changed in ");
      switch ((int)Data2) {
        case OMX_StateInvalid:
          DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateInvalid\n");
          break;
        case OMX_StateLoaded:
          DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateLoaded\n");
          break;
        case OMX_StateIdle:
          DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateIdle\n");
          break;
        case OMX_StateExecuting:
          DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateExecuting\n");
          break;
        case OMX_StatePause:
          DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StatePause\n");
          break;
        case OMX_StateWaitForResources:
          DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateWaitForResources\n");
          break;
      }
      tsem_up(appPriv->encoderEventSem);
    }
    else if (OMX_CommandPortEnable || OMX_CommandPortDisable) {
      DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s Received Port Enable/Disable Event\n",__func__);
      tsem_up(appPriv->encoderEventSem);
    }
  } else if(eEvent == OMX_EventPortSettingsChanged) {
    DEBUG(DEB_LEV_SIMPLE_SEQ, "\n port settings change event handler in %s \n", __func__);

  } else if(eEvent == OMX_EventBufferFlag) {
    DEBUG(DEFAULT_MESSAGES, "In %s OMX_BUFFERFLAG_EOS\n", __func__);
    if((int)Data2 == OMX_BUFFERFLAG_EOS) {
      tsem_up(appPriv->eofSem);
    }
  } else {
    DEBUG(DEB_LEV_SIMPLE_SEQ, "Param1 is %i\n", (int)Data1);
    DEBUG(DEB_LEV_SIMPLE_SEQ, "Param2 is %i\n", (int)Data2);
  }
#endif
  return err;
}

OMX_ERRORTYPE obs_openmax_emptybuffer(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_BUFFERHEADERTYPE* pBuffer) {

  OMX_ERRORTYPE err;
#if 0
    int data_read;
  DEBUG(DEB_LEV_FULL_SEQ, "Hi there, I am in the %s callback.\n", __func__);

  if(!flagIsCameraRequested) {
    data_read = fread(pBuffer->pBuffer, sizeof(char), buffer_in_size, infile);
    if (data_read <= 0) {
      DEBUG(DEB_LEV_SIMPLE_SEQ, "In the %s no more input data available\n", __func__);
      pBuffer->nFlags = pBuffer->nFlags | OMX_BUFFERFLAG_EOS;
      pBuffer->nFilledLen = 0;
      tsem_up(appPriv->eofSem);
      return OMX_ErrorNone;
    }
    pBuffer->nFilledLen = data_read;
    DEBUG(DEB_LEV_PARAMS, "Empty buffer %x\n", (int)pBuffer);
    err = OMX_EmptyThisBuffer(hComponent, pBuffer);
  } else if(!bEOS){
    if(pSrcOutBuffer[0]->pBuffer == pBuffer->pBuffer) {
      err = OMX_FillThisBuffer(appPriv->videosrchandle, pSrcOutBuffer[0]);
    } else {
      err = OMX_FillThisBuffer(appPriv->videosrchandle, pSrcOutBuffer[1]);
    }
  } else {
    DEBUG(DEFAULT_MESSAGES, "In %s: Dropping Fill This Buffer\n", __func__);
  }
#endif
  return OMX_ErrorNone;
}

OMX_ERRORTYPE obs_openmax_fillbuffer(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_BUFFERHEADERTYPE* pBuffer) {

  OMX_ERRORTYPE err;
#if 0
  if(pBuffer != NULL){
    if(!bEOS) {
      if((pBuffer->nFilledLen > 0)) {
          fwrite(pBuffer->pBuffer, sizeof(char),  pBuffer->nFilledLen, outfile);
          pBuffer->nFilledLen = 0;
      }
      if((pBuffer->nFlags & OMX_BUFFERFLAG_EOS) == OMX_BUFFERFLAG_EOS) {
        DEBUG(DEFAULT_MESSAGES, "In %s: eos=%x Calling Empty This Buffer\n", __func__, (int)pBuffer->nFlags);
        bEOS = OMX_TRUE;
      }
      if(!bEOS ) {
        err = OMX_FillThisBuffer(hComponent, pBuffer);
      }
    } else {
      DEBUG(DEFAULT_MESSAGES, "In %s: eos=%x Dropping Empty This Buffer\n", __func__,(int)pBuffer->nFlags);
    }
  } else {
    DEBUG(DEB_LEV_ERR, "Ouch! In %s: had NULL buffer to output...\n", __func__);
  }
#endif
  return OMX_ErrorNone;
}

OMX_CALLBACKTYPE obs_openmax_callbacks = {
	.EventHandler = obs_openmax_eventhandler,
	.EmptyBufferDone = obs_openmax_emptybuffer,
	.FillBufferDone = obs_openmax_fillbuffer
};