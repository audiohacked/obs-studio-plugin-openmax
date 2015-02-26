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

#include "obs-openmax.h"

#ifndef _STDINT_H_INCLUDED
#define _STDINT_H_INCLUDED
#endif

/* ------------------------------------------------------------------------- */
extern void *obs_openmax_create();
extern void obs_openmax_destroy();
extern bool obs_openmax_encode();

/* ------------------------------------------------------------------------- */
static const char *obs_openmax_getname(void)
{
	return "OpenMAX IL";
}

/* Encoder Plugin stuff -----------------------------------------------------*/
struct obs_encoder_info obs_openmax_plugin = {
	.id             = "obs_openmax",
	.type           = OBS_ENCODER_VIDEO,
	.codec          = "h264",
	.get_name       = obs_openmax_getname,
	.create         = obs_openmax_create,
	.destroy        = obs_openmax_destroy,
	.encode         = obs_openmax_encode,
	.update         = NULL, 
	.get_properties = NULL,
	.get_defaults   = NULL,
	.get_extra_data = NULL,
	.get_sei_data   = NULL,
	.get_video_info = NULL,
};
