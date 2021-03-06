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

bool openmax_update(void *data, obs_data_t *settings)
{// updates encoder info
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(settings);
	return false;
}

bool openmax_extra_data(void *data, uint8_t **extra_data, size_t *size)
{
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(extra_data);
	UNUSED_PARAMETER(size);
	return false;
}

bool openmax_sei(void *data, uint8_t **sei, size_t *size)
{
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(sei);
	UNUSED_PARAMETER(size);
	return false;
}

bool openmax_video_info(void *data, struct video_scale_info *info)
{
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(info);
	return false;
}
