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

#include <obs-module.h>

#include "openmax.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("linux-openmax", "en-US")

struct obs_encoder_info openmax_encoder = {
	.id             = "openmax-encoder",
	.type           = OBS_ENCODER_VIDEO,
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

bool obs_module_load(void)
{
	obs_register_encoder(&openmax_encoder);
	return true;
}

